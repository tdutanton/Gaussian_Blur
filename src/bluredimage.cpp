#include "bluredimage.h"

void BluredImage::process_image(const QImage &img, const OneDKernel &kernel,
                                QObject *parent) {
  QImage result = create_blured_image(img, kernel);
  if (!QThread::currentThread()->isInterruptionRequested()) {
    blured_image_ = result;
    emit process_complete();
  } else {
    emit show_error("Обработка отменена");
    emit process_complete();
  }
}

bool BluredImage::need_bpp_convert(const BPP &bpp) {
  return (bpp == BPP::bpp_8);
}

void BluredImage::convert_bpp(QImage &img) {
  img = img.convertToFormat(QImage::Format_RGB32);
}

QImage BluredImage::create_blured_image(const QImage &img,
                                        const OneDKernel &kernel) {
  QElapsedTimer timer;
  timer.start();
  QImage result = img;
  if (need_bpp_convert(get_image_bpp(result))) convert_bpp(result);
  emit set_max_progress(img.height() * img.width() * 2);
  int pixels_count{0};
  using BlurPassFunctionPtr =
      QImage (BluredImage::*)(const QImage &, const OneDKernel &, bool, int &);
  BlurPassFunctionPtr blur_ptr =
      (one_thread_mode_ == true) ? &BluredImage::apply_blur_pass_one_thread
                                 : &BluredImage::apply_blur_pass_multithread;

  if (!QThread::currentThread()->isInterruptionRequested())
    result = (this->*blur_ptr)(result, kernel, true, pixels_count);
  if (!QThread::currentThread()->isInterruptionRequested())
    result = (this->*blur_ptr)(result, kernel, false, pixels_count);
  quint64 elapsed = timer.elapsed();
  emit(send_blur_time(elapsed));
  return result;
}

QImage BluredImage::apply_blur_pass_multithread(const QImage &img,
                                                const OneDKernel &kernel,
                                                bool horizontal, int &pixels) {
  QImage result(img.size(), img.format());

  int img_height = img.height();
  int img_width = img.width();
  int kernel_size = kernel.get_kernel_size();
  int radius = kernel_size / 2;

  std::vector<double> kernel_values(kernel_size);
  for (int i = 0; i < kernel_size; i++) {
    kernel_values[i] = kernel.get_kernel_value(i);
  }

  // Используем блочную обработку
  const int block_size = 16;  // Размер блока для обработки
  const int blocks_x = (img_width + block_size - 1) / block_size;
  const int blocks_y = (img_height + block_size - 1) / block_size;
  const int total_blocks = blocks_x * blocks_y;

  std::atomic<int> processed_blocks{0};
  std::atomic<int> processed_pixels{0};

  // Используем мьютекс только для отправки сигналов
  QMutex progress_mutex;
  QElapsedTimer progress_timer;
  progress_timer.start();

  auto process_block = [&](int block_idx) {
    if (QThread::currentThread()->isInterruptionRequested()) return;

    int block_y = block_idx / blocks_x;
    int block_x = block_idx % blocks_x;

    int y_start = block_y * block_size;
    int y_end = std::min(y_start + block_size, img_height);
    int x_start = block_x * block_size;
    int x_end = std::min(x_start + block_size, img_width);

    int local_pixel_count = 0;

    for (int y = y_start; y < y_end; y++) {
      QRgb *result_line = reinterpret_cast<QRgb *>(result.scanLine(y));

      for (int x = x_start; x < x_end; x++) {
        double r = 0.0, g = 0.0, b = 0.0;

        for (int k = -radius; k <= radius; k++) {
          int px{0}, py{0};
          if (horizontal) {
            px = qBound(0, x + k, img_width - 1);
            py = y;
          } else {
            px = x;
            py = qBound(0, y + k, img_height - 1);
          }

          const QRgb *source_line =
              reinterpret_cast<const QRgb *>(img.scanLine(py));
          QRgb pixel = source_line[px];

          double weight = kernel_values[k + radius];
          apply_kernel_on_pixel(weight, pixel, r, g, b);
        }

        result_line[x] = qRgb(r, g, b);
        local_pixel_count++;
      }
    }

    // Увеличиваем счетчик обработанных пикселей
    processed_pixels.fetch_add(local_pixel_count, std::memory_order_relaxed);

    // Увеличиваем счетчик обработанных блоков
    int blocks_done =
        processed_blocks.fetch_add(1, std::memory_order_relaxed) + 1;

    // Отправляем сигнал прогресса только каждые 5% или если прошло 200мс
    if (blocks_done == total_blocks || blocks_done % (total_blocks / 20) == 0 ||
        progress_timer.elapsed() > 200) {
      QMutexLocker locker(&progress_mutex);
      if (progress_timer.elapsed() > 200) {
        emit send_progress(pixels + processed_pixels.load());
        progress_timer.restart();
      }
    }
  };

  // Создаем вектор индексов блоков
  QVector<int> block_indices;
  for (int i = 0; i < total_blocks; i++) {
    block_indices.append(i);
  }

  // Запускаем параллельную обработку блоков
  QtConcurrent::blockingMap(block_indices, process_block);

  // Обновляем общий счетчик пикселей
  pixels += processed_pixels.load();

  // Финальное обновление прогресса
  emit send_progress(pixels);

  if (QThread::currentThread()->isInterruptionRequested()) return img;
  return result;
}

QImage BluredImage::apply_blur_pass_one_thread(const QImage &img,
                                               const OneDKernel &kernel,
                                               bool horizontal, int &pixels) {
  QImage result = img;

  int img_height = img.height();
  int img_width = img.width();

  int max_size = img.height() * img.width() * 2;
  int step = max_size / 100;

  int kernel_size = kernel.get_kernel_size();
  int radius = kernel_size / 2;
  for (int y = 0; y < img_height; y++) {
    for (int x = 0; x < img_width; x++) {
      if (!QThread::currentThread()->isInterruptionRequested()) {
        double r = 0, g = 0, b = 0;
        for (int i = -radius; i <= radius; i++) {
          int px, py;
          if (horizontal) {
            px = qBound(0, x + i, img_width - 1);
            py = y;
          } else {
            px = x;
            py = qBound(0, y + i, img_height - 1);
          }
          QRgb pixel = img.pixel(px, py);
          double weight = kernel.get_kernel_value(i + radius);
          apply_kernel_on_pixel(weight, pixel, r, g, b);
        }
        pixels++;

        if (pixels % step == 0 || pixels >= max_size)
          emit send_progress(pixels);
        result.setPixel(x, y, qRgb(r, g, b));
      } else {
        return img;
      }
    }
  }
  return result;
}

void BluredImage::apply_kernel_on_pixel(const double &weight, QRgb &pixel,
                                        double &r, double &g, double &b) {
  r += qRed(pixel) * weight;
  g += qGreen(pixel) * weight;
  b += qBlue(pixel) * weight;
}

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
  QImage result = img;
  if (need_bpp_convert(get_image_bpp(result))) convert_bpp(result);
  emit set_max_progress(img.height() * img.width() * 2);
  int pixels_count{0};
  if (!QThread::currentThread()->isInterruptionRequested())
    result = apply_blur_pass(result, kernel, true, pixels_count);
  if (!QThread::currentThread()->isInterruptionRequested())
    result = apply_blur_pass(result, kernel, false, pixels_count);

  return result;
}

QImage BluredImage::apply_blur_pass(const QImage &img, const OneDKernel &kernel,
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
          r += qRed(pixel) * weight;
          g += qGreen(pixel) * weight;
          b += qBlue(pixel) * weight;
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

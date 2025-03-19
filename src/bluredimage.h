#ifndef BLUREDIMAGE_H
#define BLUREDIMAGE_H

#include <QCoreApplication>
#include <QFuture>
#include <QObject>
#include <QThread>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

#include "common_utils.h"
#include "modaldialog.h"
#include "onedkernel.h"
#include "progressbarblur.h"

class BluredImage : public QObject {
  Q_OBJECT
 public:
  ~BluredImage() = default;

  void process_image(const QImage &img, const OneDKernel &kernel,
                     QObject *parent);

  bool need_bpp_convert(const BPP &bpp);
  QImage get_image() const { return blured_image_; }
  QImage create_blured_image(const QImage &img, const OneDKernel &kernel);
  QImage apply_blur_pass_multithread(const QImage &img,
                                     const OneDKernel &kernel, bool horizontal,
                                     int &pixels);
  QImage apply_blur_pass_one_thread(const QImage &img, const OneDKernel &kernel,
                                    bool horizontal, int &pixels);
  void convert_bpp(QImage &img);
  void apply_kernel_on_pixel(const double &weight, QRgb &pixel, double &r,
                             double &g, double &b);
  void is_one_thread_mode(const bool &check) {
    one_thread_mode_ = check ? true : false;
  };

  using BlurPassFunctionPtr = QImage (BluredImage::*)(const QImage &,
                                                      const OneDKernel &, bool,
                                                      int &);
 signals:
  void send_progress(quint64 value);
  void set_max_progress(quint64 value);
  void processing_cancelled();
  void process_complete();
  void show_error(const QString &str);
  void send_blur_time(quint64 value);

 private:
  QImage blured_image_;
  bool one_thread_mode_ = true;
};

#endif  // BLUREDIMAGE_H

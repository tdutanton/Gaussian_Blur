#ifndef BLUREDIMAGE_H
#define BLUREDIMAGE_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QWidget>

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
  QImage apply_blur_pass(const QImage &img, const OneDKernel &kernel,
                         bool horizontal, int &pixels);
  void convert_bpp(QImage &img);

 signals:
  void send_progress(quint64 value);
  void set_max_progress(quint64 value);
  void processing_cancelled();
  void process_complete();
  void show_error(const QString &str);

 private:
  QImage blured_image_;
};

#endif  // BLUREDIMAGE_H

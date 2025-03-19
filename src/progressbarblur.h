#ifndef PROGRESSBARBLUR_H
#define PROGRESSBARBLUR_H

#include <QDebug>
#include <QIcon>
#include <QObject>
#include <QProgressDialog>
#include <QTimer>
#include <QWidget>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class ProgressBarBlur : public QWidget {
  Q_OBJECT
 public:
  explicit ProgressBarBlur(QWidget* parent = nullptr);
  void run_progress();
  void update_progress();

 private:
  quint64 max_progress_ = 100;
  QProgressDialog* process_ = nullptr;
  bool is_cancelled_ = false;

 public slots:
  void get_max_progress(quint64 value);
  void update_process(quint64 value);

 signals:
  void click_cancel();
};

#endif  // PROGRESSBARBLUR_H

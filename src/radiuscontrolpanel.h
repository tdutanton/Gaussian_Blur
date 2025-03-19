#ifndef RADIUSCONTROLPANEL_H
#define RADIUSCONTROLPANEL_H

#include <QBoxLayout>
#include <QDebug>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QWidget>

#include "common_utils.h"
#include "modaldialog.h"

class RadiusControlPanel : public QWidget {
  Q_OBJECT
 public:
  explicit RadiusControlPanel(QWidget *parent = nullptr);
  unsigned short get_radius() const;
  bool is_one_thread_mode() const { return one_thread_mode_; }

 signals:
  void radius_changed(unsigned short radius);
  void threading_change(bool is_one_thread);

 public slots:
  void get_blur_time(quint64 value);

 private slots:
  void on_radius_button_clicked();
  void on_one_thread_clicked();
  void on_multi_thread_clicked();

 private:
  QLabel *radius_welcome_label_;
  QLabel *radius_current_info_label_;
  QLabel *blur_time_label_;
  QLineEdit *radius_enter_line_;

  QPushButton *radius_button_;

  QSettings settings_;

  QGroupBox *gbx_;
  QRadioButton *one_thread_button_;
  QRadioButton *multi_thread_buton_;

  unsigned short radius_;
  unsigned blur_time_value_ = 0;
  bool one_thread_mode_ = true;
};

#endif  // RADIUSCONTROLPANEL_H

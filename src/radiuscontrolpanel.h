#ifndef RADIUSCONTROLPANEL_H
#define RADIUSCONTROLPANEL_H

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QWidget>

#include "common_utils.h"
#include "modaldialog.h"

class RadiusControlPanel : public QWidget {
  Q_OBJECT
 public:
  explicit RadiusControlPanel(QWidget *parent = nullptr);
  unsigned short get_radius() const;

 signals:
  void radius_changed(unsigned short radius);

 private slots:
  void on_radius_button_clicked();

 private:
  QLabel *radius_welcome_label_;
  QLabel *radius_current_info_label_;
  QLineEdit *radius_enter_line_;

  QPushButton *radius_button_;

  QSettings settings_;

  unsigned short radius_;
};

#endif  // RADIUSCONTROLPANEL_H

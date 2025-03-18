#include "radiuscontrolpanel.h"

RadiusControlPanel::RadiusControlPanel(QWidget *parent)
    : QWidget(parent), settings_("Anton Evgenev", "Gaussian Blur"), radius_{0} {
  radius_ = settings_.value("radius", 2).toUInt();
  QHBoxLayout *layout = new QHBoxLayout(this);

  radius_current_info_label_ = new QLabel(this);
  radius_current_info_label_->setText(
      QString("<i>Текущий радиус: %1</i>").arg(radius_));
  radius_current_info_label_->setFrameStyle(QFrame::Panel);
  radius_current_info_label_->setAlignment(Qt::AlignCenter);
  radius_current_info_label_->setStyleSheet("QLabel { padding: 10px 10px; }");
  layout->addWidget(radius_current_info_label_);

  radius_welcome_label_ =
      new QLabel(QString("<i>Ввести радиус размытия\n(от %1 до %2)</i>")
                     .arg(min_radius_)
                     .arg(max_radius_),
                 this);
  radius_welcome_label_->setAlignment(Qt::AlignCenter);
  layout->addWidget(radius_welcome_label_);

  radius_enter_line_ = new QLineEdit(this);
  radius_enter_line_->setText(QString::number(radius_));
  layout->addWidget(radius_enter_line_);

  radius_button_ = new QPushButton("Применить", this);
  layout->addWidget(radius_button_);

  connect(radius_button_, &QPushButton::clicked, this,
          &RadiusControlPanel::on_radius_button_clicked);
  connect(radius_enter_line_, &QLineEdit::returnPressed, this,
          &RadiusControlPanel::on_radius_button_clicked);

  this->setLayout(layout);
}

unsigned short RadiusControlPanel::get_radius() const { return radius_; }

void RadiusControlPanel::on_radius_button_clicked() {
  bool check_value = false;
  unsigned value = radius_enter_line_->text().toUInt(&check_value);
  if (check_value) {
    if (value < min_radius_ || value > max_radius_)
      ModalDialog::show(dialog_type::error, err,
                        "Недопустимый диапазон радиуса");
    else {
      radius_ = value;
      radius_current_info_label_->setText(
          QString("<i>Текущий радиус: %1</i>").arg(radius_));
      settings_.setValue("radius", radius_);
      emit(radius_changed(radius_));
    }
  } else
    ModalDialog::show(dialog_type::error, err, "Ошибка ввода радиуса");
}

#include "radiuscontrolpanel.h"

RadiusControlPanel::RadiusControlPanel(QWidget *parent)
    : QWidget(parent), settings_("Anton Evgenev", "Gaussian Blur"), radius_{0} {
  radius_ = settings_.value("radius", 2).toUInt();

  QHBoxLayout *layout = new QHBoxLayout(this);

  QHBoxLayout *center_h_layout = new QHBoxLayout();
  QVBoxLayout *center_v_layout = new QVBoxLayout();

  radius_current_info_label_ = new QLabel(this);
  radius_current_info_label_->setText(
      QString("<i>Текущий радиус: %1</i>").arg(radius_));
  radius_current_info_label_->setFrameStyle(QFrame::Panel);
  radius_current_info_label_->setAlignment(Qt::AlignCenter);
  radius_current_info_label_->setStyleSheet("QLabel { padding: 10px 10px; }");
  layout->addWidget(radius_current_info_label_);

  radius_welcome_label_ =
      new QLabel(QString("<i>Введите радиус<br>(от %1 до %2)</i>")
                     .arg(min_radius_)
                     .arg(max_radius_),
                 this);
  radius_welcome_label_->setAlignment(Qt::AlignCenter);
  center_h_layout->addWidget(radius_welcome_label_);

  radius_enter_line_ = new QLineEdit(this);
  radius_enter_line_->setText(QString::number(radius_));
  center_h_layout->addWidget(radius_enter_line_);

  radius_button_ = new QPushButton("Применить", this);
  center_h_layout->addWidget(radius_button_);

  center_v_layout->addLayout(center_h_layout);

  blur_time_label_ = new QLabel(
      QString("Время выполнения размытия: %1 сек").arg(blur_time_value_));
  blur_time_label_->setAlignment(Qt::AlignCenter);
  center_v_layout->addWidget(blur_time_label_);

  layout->addLayout(center_v_layout);

  gbx_ = new QGroupBox(this);
  one_thread_button_ = new QRadioButton("&Один поток");
  multi_thread_buton_ = new QRadioButton("&Мультипоток");
  one_thread_button_->setChecked(true);
  QVBoxLayout *thread_layout = new QVBoxLayout();
  thread_layout->addWidget(one_thread_button_);
  thread_layout->addWidget(multi_thread_buton_);
  gbx_->setLayout(thread_layout);

  layout->addWidget(gbx_);

  connect(radius_button_, &QPushButton::clicked, this,
          &RadiusControlPanel::on_radius_button_clicked);
  connect(radius_enter_line_, &QLineEdit::returnPressed, this,
          &RadiusControlPanel::on_radius_button_clicked);
  connect(one_thread_button_, &QRadioButton::clicked, this,
          &RadiusControlPanel::on_one_thread_clicked);
  connect(multi_thread_buton_, &QRadioButton::clicked, this,
          &RadiusControlPanel::on_multi_thread_clicked);

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

void RadiusControlPanel::on_one_thread_clicked() {
  qDebug() << "ONE T CLCK";
  one_thread_mode_ = true;
}

void RadiusControlPanel::on_multi_thread_clicked() {
  qDebug() << "MULTI T CLCK";
  one_thread_mode_ = false;
}

void RadiusControlPanel::get_blur_time(quint64 value) {
  blur_time_value_ = value;
  blur_time_label_->setText(
      QString("Время выполнения размытия: %1 сек")
          .arg(static_cast<double>(blur_time_value_) / 1000));
}

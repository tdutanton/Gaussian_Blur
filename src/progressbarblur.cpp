#include "progressbarblur.h"

ProgressBarBlur::ProgressBarBlur(QWidget *parent) : QWidget(parent) {
  process_ =
      new QProgressDialog("Обработка изображения", "Отмена", 0, 100, this);
  process_->setWindowTitle("Обработка");
  process_->setMinimumDuration(0);
  process_->setWindowIcon(QIcon(":/img/resources/process.png"));
  process_->setAutoReset(true);
  process_->setAutoClose(true);
  process_->setWindowModality(Qt::WindowModal);

  connect(process_, &QProgressDialog::canceled,
          [this]() { emit(click_cancel()); });
}

void ProgressBarBlur::run_progress() { process_->show(); }

void ProgressBarBlur::get_max_progress(quint64 value) { max_progress_ = value; }

void ProgressBarBlur::update_process(quint64 value) {
  int normalized_progress = static_cast<int>((value * 100) / max_progress_);
  if (value >= max_progress_ || normalized_progress >= 100) {
    normalized_progress = 100;
  }
  process_->setValue(normalized_progress);
}

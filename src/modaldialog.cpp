#include "modaldialog.h"

void ModalDialog::show(dialog_type type, const QString &title,
                       const QString &message, QWidget *parent) {
  QMessageBox msg_box(parent);
  msg_box.setWindowTitle(title);
  msg_box.setText(message);
  msg_box.setStandardButtons(QMessageBox::Ok);

  if (type == dialog_type::error)
    msg_box.setWindowIcon(QIcon(":/img/resources/main_icon.png"));
  else if (type == dialog_type::info)
    msg_box.setWindowIcon(QIcon(":/img/resources/about_icon.png"));

  msg_box.exec();
}

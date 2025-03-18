#ifndef MODALDIALOG_H
#define MODALDIALOG_H

#include <QIcon>
#include <QMessageBox>
#include <QString>

#include "common_utils.h"

static const QString err = "Ошибка";
static const QString about = "О программе";

class ModalDialog {
 public:
  static void show(dialog_type type, const QString &title,
                   const QString &message, QWidget *parent = nullptr);

 private:
  ModalDialog() = delete;
};

#endif  // MODALDIALOG_H

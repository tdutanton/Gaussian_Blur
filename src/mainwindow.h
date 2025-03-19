#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QFuture>
#include <QImageReader>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QThread>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

#include "bluredimage.h"
#include "common_utils.h"
#include "modaldialog.h"
#include "progressbarblur.h"
#include "radiuscontrolpanel.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

  void create_menu_bar();
  void create_actions();
  void create_tabs();
  void create_filter_bar();
  void compose_widgets(QWidget *wgt);
  void on_tab_changed(int index);
  void add_main_picture();
  void set_blur_connections(ProgressBarBlur *bar, BluredImage *img,
                            QImage &res_img, QThread *thread,
                            OneDKernel &kernel);

  bool check_image(const QImage &img);

 private:
  QMenuBar *menu_bar_;
  QMenu *file_menu_;
  QMenu *about_menu_;

  QAction *open_action_;
  QAction *save_action_;
  QAction *exit_action_;
  QAction *about_action_;

  QLabel *raw_image_label_;
  QLabel *blured_image_label_;
  QLabel *main_page_img_;

  QTabWidget *tabs_;

  QScrollArea *raw_image_scroll_;
  QScrollArea *blured_image_scroll_;

  RadiusControlPanel *radius_panel_;

  bool is_save_available_ = true;

  unsigned short current_radius_ = 0;
  QImage original_image_;

 private slots:
  void open_image();
  void save_image();
  void blur_image();
  void open_about_tab();
  void update_radius(unsigned short radius);
  void block_saving();
  void enable_to_save();
};
#endif  // MAINWINDOW_H

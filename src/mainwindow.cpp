#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Gaussian Blur");
  resize(default_width_, default_height_);
  create_actions();
  create_menu_bar();
  create_filter_bar();
  add_main_picture();
  create_tabs();

  QWidget *cntrl_wgt = new QWidget(this);
  compose_widgets(cntrl_wgt);
  setCentralWidget(cntrl_wgt);
  this->setWindowIcon(QPixmap(":/img/resources/main_icon.png"));
}

MainWindow::~MainWindow() {}

void MainWindow::create_menu_bar() {
  menu_bar_ = this->menuBar();

  file_menu_ = menu_bar_->addMenu("&Файл");
  file_menu_->addAction(open_action_);
  file_menu_->addSeparator();
  file_menu_->addAction(save_action_);
  file_menu_->addSeparator();
  file_menu_->addAction(exit_action_);

  about_menu_ = menu_bar_->addMenu("&О программе");

  about_menu_->addAction(about_action_);
}

void MainWindow::create_actions() {
  open_action_ = new QAction("Открыть...", this);
  open_action_->setShortcut(QKeySequence::Open);
  open_action_->setStatusTip("Открыть файл");

  connect(open_action_, &QAction::triggered, this, &MainWindow::open_image);

  save_action_ = new QAction("Сохранить...", this);
  save_action_->setShortcut(QKeySequence::Save);
  save_action_->setStatusTip("Сохранить файл");
  save_action_->setDisabled(true);
  connect(save_action_, &QAction::triggered, this, &MainWindow::save_image);

  exit_action_ = new QAction("Выход", this);
  exit_action_->setShortcut(QKeySequence::Close);
  exit_action_->setStatusTip("Выход из программы");

  connect(exit_action_, &QAction::triggered, this, &MainWindow::close);

  about_action_ = new QAction("О программе", this);
  about_action_->setStatusTip("Информация о программе");

  connect(about_action_, &QAction::triggered, this,
          &MainWindow::open_about_tab);
}

void MainWindow::create_tabs() {
  tabs_ = new QTabWidget(this);

  raw_image_label_ = new QLabel(this);
  raw_image_label_->setText("Откройте изображение для обработки");
  raw_image_label_->setAlignment(Qt::AlignCenter);
  raw_image_scroll_ = new QScrollArea(this);
  raw_image_scroll_->setWidget(raw_image_label_);
  raw_image_scroll_->setWidgetResizable(true);
  tabs_->addTab(raw_image_scroll_, "Изображение");

  QWidget *blur_container = new QWidget(this);
  QVBoxLayout *blur_layout = new QVBoxLayout;
  blured_image_label_ = new QLabel(this);
  blured_image_label_->setText("Откройте изображение для обработки");
  blured_image_label_->setAlignment(Qt::AlignCenter);
  blured_image_scroll_ = new QScrollArea(this);
  blured_image_scroll_->setWidget(blured_image_label_);
  blured_image_scroll_->setWidgetResizable(true);

  blur_layout->addWidget(radius_panel_);
  blur_layout->addWidget(blured_image_scroll_);
  blur_container->setLayout(blur_layout);

  tabs_->addTab(blur_container, "Фильтр");

  connect(tabs_, &QTabWidget::currentChanged, this,
          &MainWindow::on_tab_changed);
  tabs_->setMinimumSize(tabs_->sizeHint());
}

void MainWindow::on_tab_changed(int index) {
  if (tabs_->tabText(index) == "Фильтр")
    radius_panel_->show();
  else
    radius_panel_->hide();
}

void MainWindow::add_main_picture() {
  main_page_img_ = new QLabel(this);
  QPixmap pixmap(":/img/resources/main_img.jpg");
  QPixmap scaled_img = pixmap.scaled(150, 200, Qt::KeepAspectRatio);
  main_page_img_->setPixmap(scaled_img);
  main_page_img_->setScaledContents(false);
  main_page_img_->setAlignment(Qt::AlignVCenter);
}

void MainWindow::set_blur_connections(ProgressBarBlur *bar,
                                      BluredImage *blured_img, QImage &res_img,
                                      QThread *thread, OneDKernel &kernel) {
  // cancel connections
  connect(bar, &ProgressBarBlur::click_cancel, thread,
          &QThread::requestInterruption);
  connect(bar, &ProgressBarBlur::click_cancel, this, &MainWindow::block_saving);
  connect(blured_img, &BluredImage::show_error, this,
          [](const QString &message) {
            ModalDialog::show(dialog_type::error, err, message);
          });

  // get blur time
  connect(blured_img, &BluredImage::send_blur_time, radius_panel_,
          &RadiusControlPanel::get_blur_time);

  // thread finish connections
  connect(thread, &QThread::finished, blured_img, &BluredImage::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  // set saving available after processing
  connect(blured_img, &BluredImage::process_complete, this,
          &MainWindow::enable_to_save);

  // progress bar exchng
  connect(blured_img, &BluredImage::set_max_progress, bar,
          &ProgressBarBlur::get_max_progress);
  connect(blured_img, &BluredImage::send_progress, bar,
          &ProgressBarBlur::update_process);

  // main connections
  connect(blured_img, &BluredImage::process_complete, this,
          [this, blured_img]() {
            if (!QThread::currentThread()->isInterruptionRequested()) {
              QImage blured = blured_img->get_image();
              blured_image_label_->setPixmap(QPixmap::fromImage(blured));
              if (is_save_available_) save_action_->setDisabled(false);
            }
          });

  connect(thread, &QThread::started, [blured_img, res_img, kernel]() {
    blured_img->process_image(res_img, kernel, nullptr);
  });
}

void MainWindow::create_filter_bar() {
  radius_panel_ = new RadiusControlPanel(this);
  radius_panel_->hide();

  connect(radius_panel_, &RadiusControlPanel::radius_changed, this,
          &MainWindow::update_radius);
}

void MainWindow::compose_widgets(QWidget *wgt) {
  QVBoxLayout *right_layout = new QVBoxLayout;
  right_layout->setContentsMargins(1, 1, 1, 1);
  right_layout->setSpacing(10);
  right_layout->addStretch(1);
  right_layout->addWidget(main_page_img_, 1, Qt::AlignCenter);
  right_layout->addStretch(1);

  QWidget *right_container_ = new QWidget(this);
  right_container_->setLayout(right_layout);
  right_container_->setMinimumSize(right_container_->sizeHint());

  QHBoxLayout *main_hbox_layout = new QHBoxLayout;
  main_hbox_layout->setContentsMargins(10, 10, 10, 10);
  main_hbox_layout->setSpacing(1);
  main_hbox_layout->addWidget(tabs_, 8);
  main_hbox_layout->addWidget(right_container_, 1);

  wgt->setLayout(main_hbox_layout);
}

void MainWindow::open_image() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Открыть изображение", "../../../gaussian_blur/img_examples",
      "Images (*.bmp *.jpg *.png)");
  if (!filename.isEmpty()) {
    QImage image(filename);
    if (check_image(image)) {
      original_image_ = image;
      QPixmap correct_image = QPixmap::fromImage(image);
      raw_image_label_->setPixmap(correct_image);
      blured_image_label_->setText(
          "Установите радиус размытия и примените обработку");
    }
  } else
    ModalDialog::show(dialog_type::error, err, "Ошибка загрузки изображения");
}

void MainWindow::save_image() {
  if (getPixmap(blured_image_label_).isNull()) is_save_available_ = false;
  if (!is_save_available_) {
    ModalDialog::show(dialog_type::error, err,
                      "Нет изображения для сохранения");
  } else {
    QString filename = QFileDialog::getSaveFileName(
        this, "Сохранить изображение", "",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if (filename.isEmpty()) {
      return;
    }
    QString format;
    if (filename.endsWith(".png", Qt::CaseInsensitive)) {
      format = "PNG";
    } else if (filename.endsWith(".jpg", Qt::CaseInsensitive) ||
               filename.endsWith(".jpeg", Qt::CaseInsensitive)) {
      format = "JPEG";
    } else if (filename.endsWith(".bmp", Qt::CaseInsensitive)) {
      format = "BMP";
    }
    QImage image = getPixmap(blured_image_label_).toImage();
    if (!image.save(filename, format.toUtf8().constData())) {
      ModalDialog::show(dialog_type::error, err,
                        "Ошибка сохранения изображения");
    }
  }
}

void MainWindow::blur_image() {
  if (original_image_.isNull()) {
    ModalDialog::show(dialog_type::error, err,
                      "Отсутствует файл для обработки");
    return;
  }
  if (!getPixmap(raw_image_label_).isNull()) {
    QImage new_img = getPixmap(raw_image_label_).toImage();
    OneDKernel kernel = OneDKernel(radius_panel_->get_radius());

    ProgressBarBlur *prg = new ProgressBarBlur(this);
    BluredImage *blured_img = new BluredImage();
    QThread *thread = new QThread();

    set_blur_connections(prg, blured_img, new_img, thread, kernel);
    blured_img->is_one_thread_mode(radius_panel_->is_one_thread_mode());
    blured_img->moveToThread(thread);

    prg->run_progress();
    thread->start();
  } else {
    ModalDialog::show(dialog_type::error, err,
                      "Отсутствует файл для обработки");
  }
}

void MainWindow::open_about_tab() {
  QString file_name = ":/text/resources/about.txt";
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    ModalDialog::show(dialog_type::error, err,
                      "Ошибка чтения файла о программе");
  else {
    QTextStream in(&file);
    QString about_txt = in.readAll();
    file.close();
    ModalDialog::show(dialog_type::info, about, about_txt);
  }
}

void MainWindow::update_radius(unsigned short radius) {
  current_radius_ = radius;
  blur_image();
}

void MainWindow::block_saving() { is_save_available_ = false; }

void MainWindow::enable_to_save() { is_save_available_ = true; }

bool MainWindow::check_image(const QImage &img) {
  bool result = false;
  if (img.isNull())
    ModalDialog::show(dialog_type::error, err, "Ошибка загрузки изображения");
  else {
    int bpp_depth = img.bitPlaneCount();
    if (bpp_depth == static_cast<int>(BPP::bpp_8) ||
        bpp_depth == static_cast<int>(BPP::bpp_24))
      result = true;
    else
      ModalDialog::show(dialog_type::error, err,
                        "Ошибка глубины цвета изображения");
  }
  return result;
}

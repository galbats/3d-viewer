#include "mainwindow.h"

#include "ui_mainwindow.h"

// Конструктор главного окна приложения
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setFixedSize(this->width(), this->height());
  loadSetting();
}

// Деструктор главного окна приложения
MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

// Обработчик нажатия кнопки изменения цвета фона
void MainWindow::on_backgroundColorQPushButton_clicked() {
  QColor current_color =
      ui->backgroundColorQPushButton->palette().color(QPalette::Button);
  QColor selected_color =
      QColorDialog::getColor(current_color, this, "Pick color");

  if (selected_color.isValid()) {
    ui->backgroundColorQPushButton->setStyleSheet("background-color: " +
                                                  selected_color.name());
    ui->openGLWidget->setBackgroundColor(
        selected_color.redF(), selected_color.greenF(), selected_color.blueF());
    ui->openGLWidget->update();
  }
}

// Обработчик нажатия кнопки изменения цвета ребер
void MainWindow::on_edgeColorQPushButton_clicked() {
  QColor current_color =
      ui->edgeColorQPushButton->palette().color(QPalette::Button);
  QColor selected_color =
      QColorDialog::getColor(current_color, this, "Pick color");

  if (selected_color.isValid()) {
    ui->edgeColorQPushButton->setStyleSheet("background-color: " +
                                            selected_color.name());
    ui->openGLWidget->setEdgeColor(
        selected_color.redF(), selected_color.greenF(), selected_color.blueF());
    ui->openGLWidget->update();
  }
}

// Обработчик нажатия кнопки изменения цвета вершин
void MainWindow::on_vertexColorQPushButton_clicked() {
  QColor current_color =
      ui->vertexColorQPushButton->palette().color(QPalette::Button);
  QColor selected_color =
      QColorDialog::getColor(current_color, this, "Pick color");

  if (selected_color.isValid()) {
    ui->vertexColorQPushButton->setStyleSheet("background-color: " +
                                              selected_color.name());
    ui->openGLWidget->setVertexColor(
        selected_color.redF(), selected_color.greenF(), selected_color.blueF());
    ui->openGLWidget->update();
  }
}

// Сохранение настроек приложения
void MainWindow::saveSettings() {
  QSettings settings("vitalinc&gerrickl", "3D Viewer");

  Color current_vertex_color = ui->openGLWidget->getVertexColor();
  settings.setValue("vertexColorRed", current_vertex_color.red);
  settings.setValue("vertexColorGreen", current_vertex_color.green);
  settings.setValue("vertexColorBlue", current_vertex_color.blue);

  Color current_edge_color = ui->openGLWidget->getEdgeColor();
  settings.setValue("edgeColorRed", current_edge_color.red);
  settings.setValue("edgeColorGreen", current_edge_color.green);
  settings.setValue("edgeColorBlue", current_edge_color.blue);

  double current_vertices_size = ui->openGLWidget->getVerticesSize();
  settings.setValue("verticesSize", current_vertices_size);

  double current_edges_thickness = ui->openGLWidget->getEdgesThickness();
  settings.setValue("edgeThickness", current_edges_thickness);

  int current_projection_type = ui->openGLWidget->getProjectionType();
  settings.setValue("projectionType", current_projection_type);

  int current_edges_type = ui->openGLWidget->getEdgesType();
  settings.setValue("edgesType", current_edges_type);

  int current_vertex_display_type = ui->openGLWidget->getVertexDisplayType();
  settings.setValue("vertexDisplayType", current_vertex_display_type);
}

// Загрузка всех настроек приложения
void MainWindow::loadSetting() {
  QSettings app_settings("vitalinc&gerrickl", "3D Viewer");
  loadVertexColor(app_settings);
  loadEdgeColor(app_settings);
  loadSizeAndThickness(app_settings);
  loadProjectionType(app_settings);
  loadEdgesType(app_settings);
  loadVertexDisplayType(app_settings);
}

// Сброс всех значений ползунков в начальное состояние
void MainWindow::resetSliders() {
  ui->xTranslateQSlider->setValue(0);
  ui->yTranslateQSlider->setValue(0);
  ui->zTranslateQSlider->setValue(0);

  ui->xRotateQSlider->setValue(0);
  ui->yRotateQSlider->setValue(0);
  ui->zRotateQSlider->setValue(0);

  ui->scaleQSlider->setValue(100);
}

// Загрузка цвета вершин из настроек
void MainWindow::loadVertexColor(const QSettings& settings) {
  QColor saved_vertex_color;
  saved_vertex_color.setRedF(settings.value("vertexColorRed").toFloat());
  saved_vertex_color.setGreenF(settings.value("vertexColorGreen").toFloat());
  saved_vertex_color.setBlueF(settings.value("vertexColorBlue").toFloat());
  ui->openGLWidget->setVertexColor(saved_vertex_color.redF(),
                                   saved_vertex_color.greenF(),
                                   saved_vertex_color.blueF());
  ui->vertexColorQPushButton->setStyleSheet("background-color: " +
                                            saved_vertex_color.name());
}

// Загрузка цвета ребер из настроек
void MainWindow::loadEdgeColor(const QSettings& settings) {
  QColor saved_edge_color;
  saved_edge_color.setRedF(settings.value("edgeColorRed").toFloat());
  saved_edge_color.setGreenF(settings.value("edgeColorGreen").toFloat());
  saved_edge_color.setBlueF(settings.value("edgeColorBlue").toFloat());
  ui->openGLWidget->setEdgeColor(saved_edge_color.redF(),
                                 saved_edge_color.greenF(),
                                 saved_edge_color.blueF());
  ui->edgeColorQPushButton->setStyleSheet("background-color: " +
                                          saved_edge_color.name());
}

// Загрузка размеров вершин и толщины ребер из настроек
void MainWindow::loadSizeAndThickness(const QSettings& settings) {
  ui->openGLWidget->setVerticesSize(settings.value("verticesSize").toDouble());
  ui->vertexSizeQSlider->setValue(ui->openGLWidget->getVerticesSize());
  ui->openGLWidget->setEdgesThickness(
      settings.value("edgeThickness").toDouble());
  ui->thicknessQSlider->setValue(ui->openGLWidget->getEdgesThickness());
}

// Загрузка типа проекции из настроек
void MainWindow::loadProjectionType(const QSettings& settings) {
  ui->openGLWidget->setProjectionType(
      settings.value("projectionType", 0).toInt());
  if (ui->openGLWidget->getProjectionType() == 0) {
    ui->perspectiveQRadioButton->setChecked(true);
  } else {
    ui->orthographicQRadioButton->setChecked(true);
  }
}

// Загрузка типа ребер из настроек
void MainWindow::loadEdgesType(const QSettings& settings) {
  ui->openGLWidget->setEdgesType(settings.value("edgesType", 0).toInt());
  if (ui->openGLWidget->getEdgesType() == 0) {
    ui->solidQRadioButton->setChecked(true);
  } else {
    ui->dashedQRadioButton->setChecked(true);
  }
}

// Загрузка типа отображения вершин из настроек
void MainWindow::loadVertexDisplayType(const QSettings& settings) {
  ui->openGLWidget->setVertexDisplayType(
      settings.value("vertexDisplayType").toInt());
  if (ui->openGLWidget->getVertexDisplayType() == 0) {
    ui->noneVertexQRadioButton->setChecked(true);
  } else if (ui->openGLWidget->getVertexDisplayType() == 1) {
    ui->circleVertexQRadioButton->setChecked(true);
  } else {
    ui->squareVertexQRadioButton->setChecked(true);
  }
}

// Обработчик нажатия кнопки выбора отображения вершин: None
void MainWindow::on_noneVertexQRadioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(0);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора отображения вершин: Circle
void MainWindow::on_circleVertexQRadioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(1);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора отображения вершин: Square
void MainWindow::on_squareVertexQRadioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(2);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора типа ребер: Solid
void MainWindow::on_solidQRadioButton_clicked() {
  ui->openGLWidget->setEdgesType(0);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора типа ребер: Dashed
void MainWindow::on_dashedQRadioButton_clicked() {
  ui->openGLWidget->setEdgesType(1);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора типа проекции: Perspective
void MainWindow::on_perspectiveQRadioButton_clicked() {
  ui->openGLWidget->setProjectionType(0);
  ui->openGLWidget->update();
}

// Обработчик нажатия кнопки выбора типа проекции: Orthographic
void MainWindow::on_orthographicQRadioButton_clicked() {
  ui->openGLWidget->setProjectionType(1);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка размера вершин
void MainWindow::on_vertexSizeQSlider_valueChanged(int value) {
  ui->openGLWidget->setVerticesSize(value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка толщины ребер
void MainWindow::on_thicknessQSlider_valueChanged(int value) {
  ui->openGLWidget->setEdgesThickness(value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка перемещения по оси X
void MainWindow::on_xTranslateQSlider_valueChanged(int value) {
  double translation_value = value - ui->openGLWidget->getObjectTranslate().x;
  translation_value *=
      ui->openGLWidget->getController().GetMaxVertexValue() / 100;
  ui->openGLWidget->setObjectTranslateX(value);
  ui->openGLWidget->getController().TranslateAlongX(translation_value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка перемещения по оси Y
void MainWindow::on_yTranslateQSlider_valueChanged(int value) {
  double translation_value = value - ui->openGLWidget->getObjectTranslate().y;
  translation_value *=
      ui->openGLWidget->getController().GetMaxVertexValue() / 100;
  ui->openGLWidget->setObjectTranslateY(value);
  ui->openGLWidget->getController().TranslateAlongY(translation_value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка перемещения по оси Z
void MainWindow::on_zTranslateQSlider_valueChanged(int value) {
  double translation_value = value - ui->openGLWidget->getObjectTranslate().z;
  translation_value *=
      ui->openGLWidget->getController().GetMaxVertexValue() / 100;
  ui->openGLWidget->setObjectTranslateZ(value);
  ui->openGLWidget->getController().TranslateAlongZ(translation_value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка вращения по оси X
void MainWindow::on_xRotateQSlider_valueChanged(int value) {
  double rotation_value = value - ui->openGLWidget->getObjectRotation().x;
  ui->openGLWidget->getController().RotateAroundX(rotation_value);
  ui->openGLWidget->setObjectRotationX(value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка вращения по оси Y
void MainWindow::on_yRotateQSlider_valueChanged(int value) {
  double rotation_value = value - ui->openGLWidget->getObjectRotation().y;
  ui->openGLWidget->getController().RotateAroundY(rotation_value);
  ui->openGLWidget->setObjectRotationY(value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка вращения по оси Z
void MainWindow::on_zRotateQSlider_valueChanged(int value) {
  double rotation_value = value - ui->openGLWidget->getObjectRotation().z;
  ui->openGLWidget->getController().RotateAroundZ(rotation_value);
  ui->openGLWidget->setObjectRotationZ(value);
  ui->openGLWidget->update();
}

// Обработчик изменения значения ползунка масштаба
void MainWindow::on_scaleQSlider_valueChanged(int value) {
  double scale_factor = value / ui->openGLWidget->getObjectScale();
  ui->openGLWidget->getController().ScaleModel(scale_factor);
  ui->openGLWidget->setObjectScale(value);
  ui->openGLWidget->update();
}

// Обработчик выбора GIF
void MainWindow::gifSelect() {
  QImage frame(640, 480, QImage::Format_RGB32);
  frame = ui->openGLWidget->grabFramebuffer();
  gif->addFrame(frame, 10);
  ++fps;
  if (fps == 50) {
    QString file_name =
        QFileDialog::getSaveFileName(this, "Save GIF", "", "GIF (*.gif)");
    timerGif->stop();
    gif->save(file_name);
    QFileInfo check_file(file_name);
    if (check_file.exists())
      QMessageBox::information(this, QObject::tr("Success"),
                               tr("Your task is completed sir."));
    else
      QMessageBox::critical(this, QObject::tr("Error"),
                            tr("Failed. Your task is not completed sir."));
    fps = 0;
    delete gif;
    delete timerGif;
  }
}

// Обработчик нажатия кнопки открытия изображения
void MainWindow::on_openImageButton_clicked() { form.show(); }

// Обработчик действия "Открыть файл"
void MainWindow::on_actionOpen_File_triggered() {
  QSettings settings("vitalinc&gerrickl", "3D Viewer");
  settings.clear();
  settings.sync();

  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open 3D Model"), "", tr("OBJ Files (*.obj);;All Files (*)"));
  if (!file_name.isEmpty()) {
    ui->openGLWidget->resetTransformationValues();
    resetSliders();

    QFileInfo file_info(file_name);

    s21::ModelController& open_gl_controller =
        ui->openGLWidget->getController();
    if (open_gl_controller.LoadModel(file_name.toStdString().c_str()) ==
        s21::STATUS_OK) {
      ui->fileNameQLineEdit->setText(file_info.fileName());
      ui->edgesCountQLineEdit->setText(
          QString::number(open_gl_controller.GetEdges().size()));
      ui->vertexCountQLineEdit->setText(
          QString::number(open_gl_controller.GetVertices().size()));
      ui->openGLWidget->update();
    }
  }
}

// Обработчик действия "Сохранить как JPEG"
void MainWindow::on_actionSave_png_triggered() {
  QImage image = ui->openGLWidget->grabFramebuffer();
  QString filter = "BMP (*.bmp);;JPEG (*.jpeg);;JPEG (*.jpg);;PNG (*.png)";
  QString default_file_name = "untitled";
  QString selected_filter;
  QString file_name = QFileDialog::getSaveFileName(
      this, tr("Save Image"), default_file_name, filter, &selected_filter);

  if (!file_name.isEmpty()) {
    QByteArray format = "JPEG";
    if (selected_filter.startsWith("BMP")) {
      format = "BMP";
    }

    if (QFileInfo(file_name).suffix().isEmpty()) {
      file_name += '.' + format.toLower();
    }

    if (image.save(file_name, format)) {
      QMessageBox::information(this, tr("Success"),
                               tr("Your task is completed sir."));
    } else {
      QMessageBox::critical(this, tr("Error"),
                            tr("Failed. Your task is not completed sir."));
    }
  } else {
    QMessageBox::critical(this, tr("Error"),
                          tr("Sir, the image is not saved."));
  }
}

// Обработчик действия "Сохранить как GIF"
void MainWindow::on_actionSave_as_GIF_triggered() {
  gif = new QGifImage(QSize(1000, 1000));
  timerGif = new QTimer(this);

  connect(timerGif, &QTimer::timeout, this, &MainWindow::gifSelect);

  timerGif->setInterval(100);
  timerGif->start();
}
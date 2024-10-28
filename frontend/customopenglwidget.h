#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <cmath>

#include "../backend/controller.h"

struct Color {
  float red, green, blue;
};

class CustomOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  CustomOpenGLWidget(QWidget* parent = nullptr);
  ~CustomOpenGLWidget();

 public:
  // Сброс значений трансформации объекта
  void resetTransformationValues();

  // Установка параметров
  void setEdgesType(const int& type);
  void setEdgesThickness(const int& thickness);
  void setVerticesSize(const int& size);
  void setVertexDisplayType(const int& display_type);
  void setProjectionType(const int& projection_type);
  void setVertexColor(const float& red, const float& green, const float& blue);
  void setEdgeColor(const float& red, const float& green, const float& blue);
  void setBackgroundColor(const float& red, const float& green,
                          const float& blue);
  void setObjectScale(const double& scale);
  void setObjectTranslateX(const double& translate_x);
  void setObjectTranslateY(const double& translate_y);
  void setObjectTranslateZ(const double& translate_z);
  void setObjectRotationX(const double& rotation_x);
  void setObjectRotationY(const double& rotation_y);
  void setObjectRotationZ(const double& rotation_z);

  // Получение параметров
  Color getVertexColor();
  Color getEdgeColor();
  Color getBackgroundColor();
  int getVerticesSize();
  int getEdgesThickness();
  int getProjectionType();
  int getVertexDisplayType();
  int getEdgesType();
  double getObjectScale();
  s21::ModelController& getController();
  s21::Vertex getObjectTranslate();
  s21::Vertex getObjectRotation();

 protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void paintGL() override;
  void resizeEvent(QResizeEvent* event) override;

 private:
  // Внутренние функции отрисовки
  void paintVertices();
  void paintEdges();
  void paintSolidEdges();
  void paintDashedEdges();

 private:
  s21::ModelController object_;
  QPoint last_mouse_position_;
  Color vertex_color_{0.0f, 0.0f, 0.0f};
  Color edge_color_{0.0f, 0.0f, 0.0f};
  Color background_color_{0.3f, 0.3f, 0.3f};

  int vertices_size_ = 1;        // размер вершин
  int edges_thickness_ = 1;      // толщина граней
  int projection_type_ = 1;      // 0 - perspective | 1 - orthographic
  int vertex_display_type_ = 0;  // 0 - none | 1 - circle | 2 - square
  int edges_type_ = 0;           // 0 - solid | 1 - dashed

  GLfloat x_camera_rotation_ = 30;
  GLfloat y_camera_rotation_ = 0;

  double object_scale_ = 100;
  s21::Vertex object_translation_;
  s21::Vertex object_rotation_;
};

#endif

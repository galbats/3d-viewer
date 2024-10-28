#include "customopenglwidget.h"

// Конструктор виджета OpenGL
CustomOpenGLWidget::CustomOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent) {}

// Деструктор виджета OpenGL
CustomOpenGLWidget::~CustomOpenGLWidget() {}

// Инициализация OpenGL
void CustomOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

// Обработка изменения размера виджета
void CustomOpenGLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

// Обработка события изменения размера виджета
void CustomOpenGLWidget::resizeEvent(QResizeEvent* event) {
  QOpenGLWidget::resizeEvent(event);
}

// Сброс значений трансформации объекта
void CustomOpenGLWidget::resetTransformationValues() {
  object_scale_ = 100;
  object_translation_ = {0, 0, 0};
  object_rotation_ = {0, 0, 0};
}

// Установка масштаба объекта
void CustomOpenGLWidget::setObjectScale(const double& scale) {
  object_scale_ = scale;
}

// Установка значения трансформации объекта по оси X
void CustomOpenGLWidget::setObjectTranslateX(const double& translate_x) {
  object_translation_.x = translate_x;
}

// Установка значения трансформации объекта по оси Y
void CustomOpenGLWidget::setObjectTranslateY(const double& translate_y) {
  object_translation_.y = translate_y;
}

// Установка значения трансформации объекта по оси Z
void CustomOpenGLWidget::setObjectTranslateZ(const double& translate_z) {
  object_translation_.z = translate_z;
}

// Установка значения вращения объекта по оси X
void CustomOpenGLWidget::setObjectRotationX(const double& rotation_x) {
  object_rotation_.x = rotation_x;
}

// Установка значения вращения объекта по оси Y
void CustomOpenGLWidget::setObjectRotationY(const double& rotation_y) {
  object_rotation_.y = rotation_y;
}

// Установка значения вращения объекта по оси Z
void CustomOpenGLWidget::setObjectRotationZ(const double& rotation_z) {
  object_rotation_.z = rotation_z;
}

// Установка цвета вершин
void CustomOpenGLWidget::setVertexColor(const float& red, const float& green,
                                        const float& blue) {
  vertex_color_.red = red;
  vertex_color_.green = green;
  vertex_color_.blue = blue;
}

// Установка цвета ребер
void CustomOpenGLWidget::setEdgeColor(const float& red, const float& green,
                                      const float& blue) {
  edge_color_.red = red;
  edge_color_.green = green;
  edge_color_.blue = blue;
}

// Установка цвета фона
void CustomOpenGLWidget::setBackgroundColor(const float& red,
                                            const float& green,
                                            const float& blue) {
  background_color_.red = red;
  background_color_.green = green;
  background_color_.blue = blue;
}

// Установка типа ребер
void CustomOpenGLWidget::setEdgesType(const int& type) { edges_type_ = type; }

// Установка толщины ребер
void CustomOpenGLWidget::setEdgesThickness(const int& thickness) {
  edges_thickness_ = thickness;
}

// Установка размера вершин
void CustomOpenGLWidget::setVerticesSize(const int& size) {
  vertices_size_ = size;
}

// Установка типа отображения вершин
void CustomOpenGLWidget::setVertexDisplayType(const int& display_type) {
  vertex_display_type_ = display_type;
}

// Установка типа проекции
void CustomOpenGLWidget::setProjectionType(const int& projection_type) {
  projection_type_ = projection_type;
}

// Получение цвета вершин
Color CustomOpenGLWidget::getVertexColor() { return vertex_color_; }

// Получение цвета ребер
Color CustomOpenGLWidget::getEdgeColor() { return edge_color_; }

// Получение цвета фона
Color CustomOpenGLWidget::getBackgroundColor() { return background_color_; }

// Получение размера вершин
int CustomOpenGLWidget::getVerticesSize() { return vertices_size_; }

// Получение толщины ребер
int CustomOpenGLWidget::getEdgesThickness() { return edges_thickness_; }

// Получение типа проекции
int CustomOpenGLWidget::getProjectionType() { return projection_type_; }

// Получение типа отображения вершин
int CustomOpenGLWidget::getVertexDisplayType() { return vertex_display_type_; }

// Получение типа ребер
int CustomOpenGLWidget::getEdgesType() { return edges_type_; }

// Получение масштаба объекта
double CustomOpenGLWidget::getObjectScale() { return object_scale_; }

// Получение значения трансформации объекта
s21::Vertex CustomOpenGLWidget::getObjectTranslate() {
  return object_translation_;
}

// Получение значения вращения объекта
s21::Vertex CustomOpenGLWidget::getObjectRotation() { return object_rotation_; }

// Получение контроллера модели
s21::ModelController& CustomOpenGLWidget::getController() { return object_; }

// Обработка события нажатия мыши
void CustomOpenGLWidget::mousePressEvent(QMouseEvent* event) {
  last_mouse_position_ = event->pos();
}

// Обработка события перемещения мыши
void CustomOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  int delta_x = event->position().x() - last_mouse_position_.x();
  int delta_y = event->position().y() - last_mouse_position_.y();

  if (event->buttons() & Qt::LeftButton) {
    x_camera_rotation_ += delta_y;
    y_camera_rotation_ += delta_x;
  }

  last_mouse_position_ = event->pos();
  update();
}

// Отрисовка OpenGL сцены
void CustomOpenGLWidget::paintGL() {
  glClearColor(background_color_.red, background_color_.green,
               background_color_.blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glVertexPointer(3, GL_DOUBLE, 0, object_.GetVertices().data());

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double max_vertex_value = object_.GetMaxVertexValue();
  if (projection_type_ == 0) {
    glFrustum(-max_vertex_value, max_vertex_value, -max_vertex_value,
              max_vertex_value, max_vertex_value, max_vertex_value * 1000.0f);
    glTranslatef(0.0f, 0.0f, -2 * max_vertex_value);
    glRotatef(x_camera_rotation_, 1.0f, 0.0f, 0.0f);
    glRotatef(y_camera_rotation_, 0.0f, 1.0f, 0.0f);
  } else {
    glOrtho(-max_vertex_value, max_vertex_value, -max_vertex_value,
            max_vertex_value, -max_vertex_value, 1000.0f * max_vertex_value);
    glTranslatef(0.0f, -max_vertex_value / 2, 0.0f);
  }

  glEnableClientState(GL_VERTEX_ARRAY);

  if (vertex_display_type_ != 0) {
    paintVertices();
  }
  paintEdges();

  glDisableClientState(GL_VERTEX_ARRAY);
}

// Отрисовка вершин
void CustomOpenGLWidget::paintVertices() {
  if (vertex_display_type_ == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  glPointSize(vertices_size_);
  glColor3f(vertex_color_.red, vertex_color_.green, vertex_color_.blue);
  glDrawArrays(GL_POINTS, 0, object_.GetVertices().size());
  if (vertex_display_type_ == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}

// Отрисовка ребер
void CustomOpenGLWidget::paintEdges() {
  glLineWidth(edges_thickness_);
  glColor3f(edge_color_.red, edge_color_.green, edge_color_.blue);
  if (edges_type_ == 0) {
    paintSolidEdges();
  } else {
    paintDashedEdges();
  }
}

// Отрисовка сплошных ребер
void CustomOpenGLWidget::paintSolidEdges() {
  if (object_.GetVertices().empty() || object_.GetEdges().empty()) return;

  std::vector<s21::Vertex> vertices = object_.GetVertices();
  std::vector<std::pair<unsigned, unsigned>> edges = object_.GetEdges();

  glBegin(GL_LINES);
  glColor3f(edge_color_.red, edge_color_.green, edge_color_.blue);
  for (const auto& edge : edges) {
    const s21::Vertex& vertex1 = vertices[edge.first];
    const s21::Vertex& vertex2 = vertices[edge.second];
    glVertex3d(vertex1.x, vertex1.y, vertex1.z);
    glVertex3d(vertex2.x, vertex2.y, vertex2.z);
  }
  glEnd();
}

// Отрисовка пунктирных ребер
void CustomOpenGLWidget::paintDashedEdges() {
  const float dash_length = 0.1f;
  const float gap_length = 0.1f;

  glBegin(GL_LINES);

  std::vector<s21::Vertex> vertices = object_.GetVertices();
  std::vector<std::pair<unsigned, unsigned>> edges = object_.GetEdges();

  for (const auto& edge : edges) {
    const s21::Vertex& vertex1 = vertices[edge.first];
    const s21::Vertex& vertex2 = vertices[edge.second];

    float delta_x = vertex2.x - vertex1.x;
    float delta_y = vertex2.y - vertex1.y;
    float delta_z = vertex2.z - vertex1.z;
    float edge_length =
        sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
    delta_x /= edge_length;
    delta_y /= edge_length;
    delta_z /= edge_length;

    float current_length = 0.0f;
    while (current_length < edge_length) {
      float next_dash_end = current_length + dash_length;
      if (next_dash_end > edge_length) next_dash_end = edge_length;

      glVertex3d(vertex1.x + delta_x * current_length,
                 vertex1.y + delta_y * current_length,
                 vertex1.z + delta_z * current_length);
      glVertex3d(vertex1.x + delta_x * next_dash_end,
                 vertex1.y + delta_y * next_dash_end,
                 vertex1.z + delta_z * next_dash_end);

      current_length = next_dash_end + gap_length;
    }
  }
  glEnd();
}

#include "model.h"

namespace s21 {

// Функция загрузки данных из файла
// Параметр: filepath - путь к файлу для загрузки
// Возвращает: статус загрузки (успех или неудача)
int Mesh::LoadFromFile(const std::string& filepath) noexcept {
  ClearMeshData();

  std::ifstream file(filepath);
  if (!file.is_open()) return STATUS_FAIL;

  std::string line;
  while (getline(file, line)) {
    std::istringstream line_stream(line);
    std::string lineType;
    line_stream >> lineType;

    if (lineType == "v") {
      ParseVertexLine(line_stream);  // Разбор строки вершины
    } else if (lineType == "f") {
      auto face_indices =
          ExtractIndicesFromFace(line_stream);  // Извлечение индексов грани
      CreateEdgesFromFace(face_indices);  // Создание ребер из грани
    }
  }

  file.close();
  return STATUS_OK;
}

// Функция очистки данных сетки
void Mesh::ClearMeshData() noexcept {
  max_vertex_value_ = 0;  // Сбрасываем максимальное значение вершины
  vertices_.clear();  // Очищаем вектор вершин
  edges_.clear();     // Очищаем вектор ребер
}

// Функция разбора строки с вершиной
// Параметр: line_stream - поток строки
void Mesh::ParseVertexLine(std::istringstream& line_stream) noexcept {
  Vertex vertex;
  line_stream >> vertex.x >> vertex.y >> vertex.z;  // Чтение координат вершины
  vertices_.push_back(vertex);  // Добавление вершины в вектор

  // Обновление максимального значения вершины
  max_vertex_value_ = std::max({max_vertex_value_, std::abs(vertex.x),
                                std::abs(vertex.y), std::abs(vertex.z)});
}

// Функция извлечения индексов из строки грани
// Параметр: line_stream - поток строки
// Возвращает: вектор индексов вершин
std::vector<unsigned> Mesh::ExtractIndicesFromFace(
    std::istringstream& line_stream) noexcept {
  std::vector<unsigned> face_indices;
  std::string vertex_data;

  while (line_stream >> vertex_data) {
    int index = 0;
    try {
      index = std::stoi(vertex_data.substr(0, vertex_data.find('/')));
    } catch (const std::invalid_argument&) {
      continue;  // Если не удалось преобразовать строку в число, пропускаем
    }
    index = index < 0 ? vertices_.size() + index
                      : index - 1;  // Обработка отрицательных индексов
    face_indices.push_back(index);  // Добавление индекса в вектор
  }

  return face_indices;
}

// Функция создания ребер из грани
// Параметр: face_indices - вектор индексов вершин грани
void Mesh::CreateEdgesFromFace(
    const std::vector<unsigned>& face_indices) noexcept {
  for (auto it = face_indices.begin(); it != face_indices.end(); ++it) {
    unsigned startIndex = *it;
    unsigned endIndex =
        (it + 1 != face_indices.end()) ? *(it + 1) : face_indices.front();
    if (startIndex != endIndex) {
      edges_.emplace_back(
          startIndex, endIndex);  // Создание ребра и добавление его в вектор
    }
  }
}

////////////// АФИННЫЕ ПРЕОБРАЗОВАНИЯ //////////////

// Функция применения сдвига по оси X
// Параметры: vertices - вектор вершин, value - значение сдвига
void Operation::TranslateX(std::vector<Vertex>& vertices, const double& value) {
  for (auto& v : vertices) {
    v.x += value;
  }
}

// Функция применения сдвига по оси Y
// Параметры: vertices - вектор вершин, value - значение сдвига
void Operation::TranslateY(std::vector<Vertex>& vertices, const double& value) {
  for (auto& v : vertices) {
    v.y += value;
  }
}

// Функция применения сдвига по оси Z
// Параметры: vertices - вектор вершин, value - значение сдвига
void Operation::TranslateZ(std::vector<Vertex>& vertices, const double& value) {
  for (auto& v : vertices) {
    v.z += value;
  }
}

// Функция применения поворота вокруг оси X
// Параметры: vertices - вектор вершин, value - угол поворота в градусах
void Operation::RotateX(std::vector<Vertex>& vertices, const double& value) {
  double radians = value * M_PI / 180.0;
  // Поворот вокруг оси X:
  // (x', y', z') = (x, y * cos(θ) - z * sin(θ), y * sin(θ) + z * cos(θ))
  //
  // Матрица преобразования:
  // | 1      0           0      | | x |
  // | 0  cos(θ)  -sin(θ) | | y |
  // | 0  sin(θ)   cos(θ) | | z |

  for (auto& v : vertices) {
    double new_y = v.y * cos(radians) - v.z * sin(radians);
    double new_z = v.y * sin(radians) + v.z * cos(radians);
    v.y = new_y;
    v.z = new_z;
  }
}

// Функция применения поворота вокруг оси Y
// Параметры: vertices - вектор вершин, value - угол поворота в градусах
void Operation::RotateY(std::vector<Vertex>& vertices, const double& value) {
  double radians = value * M_PI / 180.0;
  // Поворот вокруг оси Y:
  // (x', y', z') = (x * cos(θ) + z * sin(θ), y, -x * sin(θ) + z * cos(θ))
  //
  // Матрица преобразования:
  // |  cos(θ)  0  sin(θ) | | x |
  // |      0     1      0    | | y |
  // | -sin(θ)  0  cos(θ) | | z |

  for (auto& v : vertices) {
    double new_x = v.x * cos(radians) + v.z * sin(radians);
    double new_z = -v.x * sin(radians) + v.z * cos(radians);
    v.x = new_x;
    v.z = new_z;
  }
}

// Функция применения поворота вокруг оси Z
// Параметры: vertices - вектор вершин, value - угол поворота в градусах
void Operation::RotateZ(std::vector<Vertex>& vertices, const double& value) {
  double radians = value * M_PI / 180.0;
  // Поворот вокруг оси Z:
  // (x', y', z') = (x * cos(θ) - y * sin(θ), x * sin(θ) + y * cos(θ), z)
  //
  // Матрица преобразования:
  // | cos(θ)  -sin(θ)  0 | | x |
  // | sin(θ)   cos(θ)  0 | | y |
  // |    0         0       1 | | z |

  for (auto& v : vertices) {
    double new_x = v.x * cos(radians) - v.y * sin(radians);
    double new_y = v.x * sin(radians) + v.y * cos(radians);
    v.x = new_x;
    v.y = new_y;
  }
}

// Функция применения масштабирования
// Параметры: vertices - вектор вершин, value - коэффициент масштабирования
void Operation::Scale(std::vector<Vertex>& vertices, const double& value) {
  for (auto& v : vertices) {
    v.x *= value;
    v.y *= value;
    v.z *= value;
  }
}

}  // namespace s21

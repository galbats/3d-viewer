#ifndef VIEWER_MODEL_H_
#define VIEWER_MODEL_H_

#include <cmath>    // для функций синуса и косинуса
#include <fstream>  // для работы с файловыми потоками ввода/вывода
#include <sstream>  // для работы с строковыми потоками
#include <string>   // для работы со строками
#include <utility>  // для работы с парами
#include <vector>   // для работы с векторами

namespace s21 {

// Константы для статуса загрузки
constexpr int STATUS_OK = 0;
constexpr int STATUS_FAIL = 1;

// Структура для хранения координат вершины
struct Vertex {
  double x, y, z;
};

// Класс для работы с 3D сеткой
class Mesh final {
 public:
  // Загрузка данных из файла
  int LoadFromFile(const std::string& filepath) noexcept;

 public:
  // Получение максимального значения координаты вершины
  double GetMaxVertexValue() noexcept { return max_vertex_value_; };
  // Получение ссылок на векторы вершин и ребер
  std::vector<Vertex>& GetVertices() noexcept { return vertices_; };
  std::vector<std::pair<unsigned, unsigned>> GetEdges() noexcept {
    return edges_;
  };

 private:
  // Вспомогательные функции для разбора файла
  std::vector<unsigned> ExtractIndicesFromFace(
      std::istringstream& line_stream) noexcept;
  void CreateEdgesFromFace(const std::vector<unsigned>& face_indices) noexcept;
  void ParseVertexLine(std::istringstream& line_stream) noexcept;
  void ClearMeshData() noexcept;

 private:
  // Векторы для хранения вершин и ребер
  std::vector<Vertex> vertices_;
  std::vector<std::pair<unsigned, unsigned>> edges_;
  // Переменная для хранения максимального значения координаты вершины
  double max_vertex_value_ = 0;
};

////////////// АФИННЫЕ ПРЕОБРАЗОВАНИЯ //////////////

// Класс для операций преобразования
class Operation {
 public:
  // Функции для применения преобразований
  void TranslateX(std::vector<Vertex>& vertices, const double& value);
  void TranslateY(std::vector<Vertex>& vertices, const double& value);
  void TranslateZ(std::vector<Vertex>& vertices, const double& value);
  void RotateX(std::vector<Vertex>& vertices, const double& value);
  void RotateY(std::vector<Vertex>& vertices, const double& value);
  void RotateZ(std::vector<Vertex>& vertices, const double& value);
  void Scale(std::vector<Vertex>& vertices, const double& value);
};

}  // namespace s21

#endif

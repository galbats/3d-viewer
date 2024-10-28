#ifndef VIEWER_CONTROLLER_H_
#define VIEWER_CONTROLLER_H_

#include "model.h"

namespace s21 {

class ModelController {
 public:
  int LoadModel(const std::string& filepath) {
    return model_.LoadFromFile(filepath);
  };
  double GetMaxVertexValue() noexcept { return model_.GetMaxVertexValue(); };
  std::vector<Vertex> GetVertices() noexcept { return model_.GetVertices(); };
  std::vector<std::pair<unsigned, unsigned>> GetEdges() noexcept {
    return model_.GetEdges();
  };

 public:
  void TranslateAlongX(const double& value) {
    Operation operation;
    operation.TranslateX(model_.GetVertices(), value);
  };
  void TranslateAlongY(const double& value) {
    Operation operation;
    operation.TranslateY(model_.GetVertices(), value);
  };
  void TranslateAlongZ(const double& value) {
    Operation operation;
    operation.TranslateZ(model_.GetVertices(), value);
  };
  void RotateAroundX(const double& value) {
    Operation operation;
    operation.RotateX(model_.GetVertices(), value);
  };
  void RotateAroundY(const double& value) {
    Operation operation;
    operation.RotateY(model_.GetVertices(), value);
  };
  void RotateAroundZ(const double& value) {
    Operation operation;
    operation.RotateZ(model_.GetVertices(), value);
  };
  void ScaleModel(const double& value) {
    Operation operation;
    operation.Scale(model_.GetVertices(), value);
  };

 private:
  Mesh model_;
};

}  // namespace s21

#endif

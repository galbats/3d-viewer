#include <gtest/gtest.h>

#include "../backend/controller.h"

// Загрузка моделей и проверка статуса
TEST(ModelLoaderTest, ValidModels) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/gun.obj"), s21::STATUS_OK);
  ASSERT_EQ(controller.LoadModel("objects/knife.obj"), s21::STATUS_OK);
}

TEST(ModelLoaderTest, InvalidModel) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/nonexistent.obj"), s21::STATUS_FAIL);
}

// Проверка значений и размеров
TEST(ModelPropertiesTest, MaxVertexValue) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  ASSERT_NEAR(controller.GetMaxVertexValue(), 14.836, 0.001);
}

TEST(ModelPropertiesTest, EdgeCount) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  ASSERT_EQ(controller.GetEdges().size(), static_cast<size_t>(3423));
}

TEST(ModelPropertiesTest, VertexCount) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  ASSERT_EQ(controller.GetVertices().size(), static_cast<size_t>(642));
}

// Тесты аффинных преобразований
TEST(TransformationTests, ShiftX) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  const double shiftX = 125.0;
  auto initialVertices = controller.GetVertices();
  controller.TranslateAlongX(shiftX);

  auto vertices = controller.GetVertices();
  for (size_t i = 0; i < vertices.size(); ++i) {
    EXPECT_TRUE(std::abs(vertices[i].x - (initialVertices[i].x + shiftX)) <
                    1e-6 &&
                std::abs(vertices[i].y - initialVertices[i].y) < 1e-6 &&
                std::abs(vertices[i].z - initialVertices[i].z) < 1e-6);
  }
}

TEST(TransformationTests, ShiftY) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  const double shiftY = 4.0;
  auto initialVertices = controller.GetVertices();
  controller.TranslateAlongY(shiftY);

  auto vertices = controller.GetVertices();
  for (size_t i = 0; i < vertices.size(); ++i) {
    EXPECT_TRUE(std::abs(vertices[i].x - initialVertices[i].x) < 1e-6 &&
                std::abs(vertices[i].y - (initialVertices[i].y + shiftY)) <
                    1e-6 &&
                std::abs(vertices[i].z - initialVertices[i].z) < 1e-6);
  }
}

TEST(TransformationTests, ShiftZ) {
  s21::ModelController controller;
  controller.LoadModel("objects/gun.obj");
  const double shiftZ = 6.0;
  auto initialVertices = controller.GetVertices();
  controller.TranslateAlongZ(shiftZ);

  auto vertices = controller.GetVertices();
  for (size_t i = 0; i < vertices.size(); ++i) {
    EXPECT_TRUE(std::abs(vertices[i].x - initialVertices[i].x) < 1e-6 &&
                std::abs(vertices[i].y - initialVertices[i].y) < 1e-6 &&
                std::abs(vertices[i].z - (initialVertices[i].z + shiftZ)) <
                    1e-6);
  }
}

TEST(TransformationTests, RotateAroundX) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/gun.obj"), s21::STATUS_OK);

  double angleX = 45.0;
  double radiansX = angleX * M_PI / 180.0;

  auto vertices_before = controller.GetVertices();
  double x_before = vertices_before[0].x;
  double y_before = vertices_before[0].y;
  double z_before = vertices_before[0].z;

  double y_after_expected = y_before * cos(radiansX) - z_before * sin(radiansX);
  double z_after_expected = y_before * sin(radiansX) + z_before * cos(radiansX);

  controller.RotateAroundX(angleX);

  auto vertices_after = controller.GetVertices();
  double x_after = vertices_after[0].x;
  double y_after = vertices_after[0].y;
  double z_after = vertices_after[0].z;

  EXPECT_NEAR(x_after, x_before, 1e-6);
  EXPECT_NEAR(y_after, y_after_expected, 1e-6);
  EXPECT_NEAR(z_after, z_after_expected, 1e-6);
}

TEST(TransformationTests, RotateAroundY) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/gun.obj"), s21::STATUS_OK);

  double angleY = 45.0;
  double radiansY = angleY * M_PI / 180.0;

  auto vertices_before = controller.GetVertices();
  double x_before = vertices_before[0].x;
  double y_before = vertices_before[0].y;
  double z_before = vertices_before[0].z;

  double x_after_expected = x_before * cos(radiansY) + z_before * sin(radiansY);
  double z_after_expected =
      -x_before * sin(radiansY) + z_before * cos(radiansY);

  controller.RotateAroundY(angleY);

  auto vertices_after = controller.GetVertices();
  double x_after = vertices_after[0].x;
  double y_after = vertices_after[0].y;
  double z_after = vertices_after[0].z;

  EXPECT_NEAR(x_after, x_after_expected, 1e-6);
  EXPECT_NEAR(y_after, y_before, 1e-6);
  EXPECT_NEAR(z_after, z_after_expected, 1e-6);
}

TEST(TransformationTests, RotateAroundZ) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/gun.obj"), s21::STATUS_OK);

  double angleZ = 45.0;
  double radiansZ = angleZ * M_PI / 180.0;

  auto vertices_before = controller.GetVertices();
  double x_before = vertices_before[0].x;
  double y_before = vertices_before[0].y;
  double z_before = vertices_before[0].z;

  double x_after_expected = x_before * cos(radiansZ) - y_before * sin(radiansZ);
  double y_after_expected = x_before * sin(radiansZ) + y_before * cos(radiansZ);

  controller.RotateAroundZ(angleZ);

  auto vertices_after = controller.GetVertices();
  double x_after = vertices_after[0].x;
  double y_after = vertices_after[0].y;
  double z_after = vertices_after[0].z;

  EXPECT_NEAR(x_after, x_after_expected, 1e-6);
  EXPECT_NEAR(y_after, y_after_expected, 1e-6);
  EXPECT_NEAR(z_after, z_before, 1e-6);
}

TEST(TransformationTests, ScaleModel) {
  s21::ModelController controller;
  ASSERT_EQ(controller.LoadModel("objects/gun.obj"), s21::STATUS_OK);

  double scaleFactor = 2.0;

  auto vertices_before = controller.GetVertices();
  double x_before = vertices_before[0].x;
  double y_before = vertices_before[0].y;
  double z_before = vertices_before[0].z;

  controller.ScaleModel(scaleFactor);

  auto vertices_after = controller.GetVertices();
  double x_after = vertices_after[0].x;
  double y_after = vertices_after[0].y;
  double z_after = vertices_after[0].z;

  double x_after_expected = x_before * scaleFactor;
  double y_after_expected = y_before * scaleFactor;
  double z_after_expected = z_before * scaleFactor;

  EXPECT_NEAR(x_after, x_after_expected, 1e-6);
  EXPECT_NEAR(y_after, y_after_expected, 1e-6);
  EXPECT_NEAR(z_after, z_after_expected, 1e-6);
}

#include "ray_tracer/ray_tracer.h"

#include "shader/material.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace {

class TestSceneNode : public SceneNode {
 public:
  TestSceneNode(bool find_intersection, bool has_intersection,
                const Material* material)
      : find_intersection_(find_intersection),
        has_intersection_(has_intersection),
        material_(material) {
  }

  void CalculateTransforms(const Matrix4& parent_transform) {
  }

  bool FindIntersection(const Ray& ray, double* t, Point3* point,
                        Vector3* normal, const Material** material) const {
    if (find_intersection_) {
      *material = material_;
      find_intersection_ = false;
      return true;
    }

    return false;
  }

  bool HasIntersection(const Ray& ray, double max_t) const {
    return has_intersection_;
  }

 private:
  mutable bool find_intersection_;
  const bool has_intersection_;
  const Material* material_;
};

class TestRayTracer : public RayTracer {
 public:
  TestRayTracer(const std::shared_ptr<const TransformNode>& root,
                const std::shared_ptr<const std::vector<std::shared_ptr<Light>>>& lights)
      : RayTracer(root, lights) {
  }

  using RayTracer::GetColor;
  using RayTracer::GetAbsorbedColor;
  using RayTracer::GetReflectedColor;
};

}  // namespace

class RayTracerTest : public testing::Test {
 public:
  RayTracerTest() : ray_(point_, normal_) {
  }

 protected:
  void SetUp() {
    root_.reset(new TransformNode());
    lights_.reset(new std::vector<std::shared_ptr<Light>>());
  }

  std::shared_ptr<TransformNode> root_;
  std::shared_ptr<std::vector<std::shared_ptr<Light>>> lights_;

  Ray ray_;
  Point3 point_;
  Vector3 normal_;
};

TEST_F(RayTracerTest, GetColor) {
  Material material(std::make_shared<Color>(1.0, 1.0, 1.0), 20, 0.5);
  root_->AddChild(std::make_shared<TestSceneNode>(true, false, &material));
  TestRayTracer ray_tracer(root_, lights_);

  Color color = ray_tracer.GetColor(ray_);
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST_F(RayTracerTest, GetColorNoIntersection) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, false, nullptr));
  TestRayTracer ray_tracer(root_, lights_);

  Color color = ray_tracer.GetColor(ray_);
  EXPECT_TRUE(color == Color());
}

TEST_F(RayTracerTest, GetAbsorbedColor) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, false, nullptr));
  lights_->push_back(std::make_shared<Light>(
      std::make_shared<Point3>(1.0, 0.0, 0.0),
      std::make_shared<Color>(0.4, 0.4, 0.4)));
  TestRayTracer ray_tracer(root_, lights_);

  Vector3 normal(1.0, 0.0, 0.0);
  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal, material.get());
  EXPECT_TRUE(color == Color(0.25, 0.25, 0.25));
}

TEST_F(RayTracerTest, GetAbsorbedColorNoLights) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, false, nullptr));
  TestRayTracer ray_tracer(root_, lights_);

  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal_, material.get());
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST_F(RayTracerTest, GetAbsorbedColorShadows) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, true, nullptr));
  lights_->push_back(std::make_shared<Light>(
      std::make_shared<Point3>(),
      std::make_shared<Color>(1.0, 1.0, 1.0)));
  TestRayTracer ray_tracer(root_, lights_);

  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal_, material.get());
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST_F(RayTracerTest, GetReflectedColor) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, false, nullptr));
  lights_->push_back(std::make_shared<Light>(
      std::make_shared<Point3>(-1.0, 0.0, 0.0),
      std::make_shared<Color>(0.4, 0.4, 0.4)));
  TestRayTracer ray_tracer(root_, lights_);

  Vector3 direction(-1.0, 0.0, 0.0);
  Ray ray(Point3(), direction);
  Vector3 normal(0.0, 1.0, 0.0);
  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(), 20, 0));
  Color color = ray_tracer.GetReflectedColor(ray, point_, normal, material.get());
  EXPECT_TRUE(color == Color(0.4, 0.4, 0.4));
}

TEST_F(RayTracerTest, GetReflectedColorNoLights) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, false, nullptr));
  TestRayTracer ray_tracer(root_, lights_);

  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(), 10, 0));
  Color color = ray_tracer.GetReflectedColor(ray_, point_, normal_, material.get());
  EXPECT_TRUE(color == Color());
}

TEST_F(RayTracerTest, GetReflectedColorShadows) {
  root_->AddChild(std::make_shared<TestSceneNode>(false, true, nullptr));
  lights_->push_back(std::make_shared<Light>(
      std::make_shared<Point3>(),
      std::make_shared<Color>(1.0, 1.0, 1.0)));
  TestRayTracer ray_tracer(root_, lights_);

  std::shared_ptr<Material> material(new Material(
      std::make_shared<Color>(), 10, 0));
  Color color = ray_tracer.GetReflectedColor(ray_, point_, normal_, material.get());
  EXPECT_TRUE(color == Color());
}

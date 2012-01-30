#include "altadore/ray_tracer/ray_tracer.h"

#include "altadore/visual/material.h"
#include "bonavista/testing/test_case.h"

namespace {

class TestSceneNode : public SceneNode {
 public:
  TestSceneNode(bool find_intersection, bool has_intersection, const Material* material)
      : find_intersection_(find_intersection), has_intersection_(has_intersection), material_(material) {
  }

  void CalculateTransforms(const Matrix4& parent_transform) {
  }

  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const {
    if (find_intersection_) {
      *material = material_;
      find_intersection_ = false;
      return true;
    }

    return false;
  }

  bool HasIntersection(const Ray& ray) const {
    return has_intersection_;
  }

 private:
  mutable bool find_intersection_;
  const bool has_intersection_;
  const Material* material_;
};

class TestRayTracer : public RayTracer {
 public:
  TestRayTracer(const TransformNode* root, const LightVector* lights) : RayTracer(root, lights) {
  }

  using RayTracer::GetColor;
  using RayTracer::GetAbsorbedColor;
  using RayTracer::GetReflectedColor;
};

}  // namespace

TEST_CASE(RayTracerTest) {
 public:
  RayTracerTest() : ray_(point_, normal_) {
  }

 protected:
  void SetUp() {
    root_.Reset(new TransformNode());
    lights_.Reset(new LightVector());
  }

  memory::scoped_refptr<TransformNode> root_;
  memory::scoped_refptr<LightVector> lights_;

  Ray ray_;
  Point3 point_;
  Vector3 normal_;
};

TEST(RayTracerTest, GetColor) {
  Material material(new Color(1.0, 1.0, 1.0), 20, 0.5);
  root_->AddChild(new TestSceneNode(true, false, &material));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  Color color = ray_tracer.GetColor(ray_);
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST(RayTracerTest, GetColorNoIntersection) {
  root_->AddChild(new TestSceneNode(false, false, NULL));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  Color color = ray_tracer.GetColor(ray_);
  EXPECT_TRUE(color == Color());
}

TEST(RayTracerTest, GetAbsorbedColor) {
  root_->AddChild(new TestSceneNode(false, false, NULL));
  lights_->AddLight(new Light(new Point3(1.0, 0.0, 0.0), new Color(0.4, 0.4, 0.4)));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  Vector3 normal(1.0, 0.0, 0.0);
  memory::scoped_refptr<Material> material(new Material(new Color(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal, material.ptr());
  EXPECT_TRUE(color == Color(0.25, 0.25, 0.25));
}

TEST(RayTracerTest, GetAbsorbedColorNoLights) {
  root_->AddChild(new TestSceneNode(false, false, NULL));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  memory::scoped_refptr<Material> material(new Material(new Color(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal_, material.ptr());
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST(RayTracerTest, GetAbsorbedColorShadows) {
  root_->AddChild(new TestSceneNode(false, true, NULL));
  lights_->AddLight(new Light(new Point3(), new Color(1.0, 1.0, 1.0)));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  memory::scoped_refptr<Material> material(new Material(new Color(0.5, 0.5, 0.5), 0, 0));
  Color color = ray_tracer.GetAbsorbedColor(point_, normal_, material.ptr());
  EXPECT_TRUE(color == Color(0.05, 0.05, 0.05));
}

TEST(RayTracerTest, GetReflectedColor) {
  root_->AddChild(new TestSceneNode(false, false, NULL));
  lights_->AddLight(new Light(new Point3(-1.0, 0.0, 0.0), new Color(0.4, 0.4, 0.4)));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  Vector3 direction(-1.0, 0.0, 0.0);
  Ray ray(Point3(), direction);
  Vector3 normal(0.0, 1.0, 0.0);
  memory::scoped_refptr<Material> material(new Material(new Color(), 20, 0));
  Color color = ray_tracer.GetReflectedColor(ray, point_, normal, material.ptr());
  EXPECT_TRUE(color == Color(0.4, 0.4, 0.4));
}

TEST(RayTracerTest, GetReflectedColorNoLights) {
  root_->AddChild(new TestSceneNode(false, false, NULL));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  memory::scoped_refptr<Material> material(new Material(new Color(), 10, 0));
  Color color = ray_tracer.GetReflectedColor(ray_, point_, normal_, material.ptr());
  EXPECT_TRUE(color == Color());
}

TEST(RayTracerTest, GetReflectedColorShadows) {
  root_->AddChild(new TestSceneNode(false, true, NULL));
  lights_->AddLight(new Light(new Point3(), new Color(1.0, 1.0, 1.0)));
  TestRayTracer ray_tracer(root_.ptr(), lights_.ptr());

  memory::scoped_refptr<Material> material(new Material(new Color(), 10, 0));
  Color color = ray_tracer.GetReflectedColor(ray_, point_, normal_, material.ptr());
  EXPECT_TRUE(color == Color());
}

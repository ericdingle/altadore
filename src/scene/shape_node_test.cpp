#include "scene/shape_node.h"

#include "shape/shape.h"
#include "shader/color.h"
#include "shader/material.h"
#include "shape/cube.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace {

class TestShape : public Shape {
 public:
  TestShape(bool intersection) : intersection_(intersection) {
  }

  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const {
    *t = 1;
    *point = ray.origin();
    *normal = ray.direction();
    return intersection_;
  }

  bool HasIntersection(const Ray& ray, double max_t) const {
    return intersection_;
  }

 private:
  bool intersection_;
};

class TestShapeNode : public ShapeNode {
 public:
  TestShapeNode(const std::shared_ptr<const Shape>& shape,
                const std::shared_ptr<const Material>& material) : ShapeNode(shape, material) {
  }

  using ShapeNode::transform;
  using ShapeNode::transform_inverse;
  using ShapeNode::transform_inverse_transpose;
};

}  // namespace

class ShapeNodeTest : public testing::Test {
 protected:
  void SetUp() {
    material_ = std::make_shared<Material>(std::make_shared<Color>(), 1, 1);
  }

  std::shared_ptr<Material> material_;
};

TEST_F(ShapeNodeTest, CalcTransforms) {
  Matrix4 transform = Matrix4::GetScaling(2);

  TestShapeNode node(std::make_shared<TestShape>(true), material_);
  node.CalculateTransforms(transform);
  EXPECT_TRUE(node.transform() == transform);
  EXPECT_TRUE(node.transform_inverse() == transform.GetInverse());
  EXPECT_TRUE(node.transform_inverse_transpose() == transform.GetInverse().GetTranspose());
}

TEST_F(ShapeNodeTest, FindIntersection) {
  Point3 origin(1, 1, 1);
  Vector3 direction(1, 0, 0);
  Ray ray(origin, direction);

  double t = 0;
  Point3 point;
  Vector3 normal;
  const Material* material;

  ShapeNode node1(std::make_shared<TestShape>(true), material_);
  node1.CalculateTransforms(Matrix4::GetScaling(2));
  EXPECT_TRUE(node1.FindIntersection(ray, &t, &point, &normal, &material));
  EXPECT_EQ(1, t);
  EXPECT_TRUE(point == Point3(1, 1, 1));
  EXPECT_TRUE(normal == Vector3(1, 0, 0));
  EXPECT_EQ(material_.get(), material);

  ShapeNode node2(std::make_shared<TestShape>(false), material_);
  EXPECT_FALSE(node2.FindIntersection(ray, &t, &point, &normal, &material));
}

TEST_F(ShapeNodeTest, HasIntersection) {
  Point3 point;
  Ray ray(point, Vector3());

  ShapeNode node1(std::make_shared<TestShape>(true), material_);
  EXPECT_TRUE(node1.HasIntersection(ray, 0));

  ShapeNode node2(std::make_shared<TestShape>(false), material_);
  EXPECT_FALSE(node2.HasIntersection(ray, 0));
}

#include "altadore/scene/shape_node.h"

#include "altadore/shape/shape.h"
#include "altadore/shader/color.h"
#include "altadore/shader/material.h"
#include "altadore/shape/cube.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

namespace {

class TestShape : public Shape {
 public:
  TestShape(bool intersection) : intersection_(intersection) {
  }

  Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var) {
    return RESULT_ERR_NAME;
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

  Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var) {
    return RESULT_ERR_NAME;
  }

  using ShapeNode::transform;
  using ShapeNode::transform_inverse;
  using ShapeNode::transform_inverse_transpose;
};

}  // namespace

TEST_CASE(ShapeNodeTest) {
 protected:
  void SetUp() {
    material_.reset(new Material(std::make_shared<Color>(), 1, 1));
  }

  std::shared_ptr<Material> material_;
};

TEST(ShapeNodeTest, Create) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Cube());
  var.reset(new Variant(object));
  args.push_back(var);

  object.reset(new Material(std::make_shared<Color>(), 1, 1));
  var.reset(new Variant(object));
  args.push_back(var);

  EXPECT_EQ(ShapeNode::Create(args, &object), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());
}

TEST(ShapeNodeTest, CreateError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(ShapeNode::Create(args, &object),
            Invokable::RESULT_ERR_ARG_SIZE);

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(ShapeNode::Create(args, &object),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(ShapeNodeTest, CalcTransforms) {
  Matrix4 transform = Matrix4::GetScaling(2);

  TestShapeNode node(std::make_shared<TestShape>(true), material_);
  node.CalculateTransforms(transform);
  EXPECT_TRUE(node.transform() == transform);
  EXPECT_TRUE(node.transform_inverse() == transform.GetInverse());
  EXPECT_TRUE(node.transform_inverse_transpose() == transform.GetInverse().GetTranspose());
}

TEST(ShapeNodeTest, FindIntersection) {
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
  EXPECT_EQ(t, 1);
  EXPECT_TRUE(point == Point3(1, 1, 1));
  EXPECT_TRUE(normal == Vector3(1, 0, 0));
  EXPECT_EQ(material, material_.get());

  ShapeNode node2(std::make_shared<TestShape>(false), material_);
  EXPECT_FALSE(node2.FindIntersection(ray, &t, &point, &normal, &material));
}

TEST(ShapeNodeTest, HasIntersection) {
  Point3 point;
  Ray ray(point, Vector3());

  ShapeNode node1(std::make_shared<TestShape>(true), material_);
  EXPECT_TRUE(node1.HasIntersection(ray, 0));

  ShapeNode node2(std::make_shared<TestShape>(false), material_);
  EXPECT_FALSE(node2.HasIntersection(ray, 0));
}

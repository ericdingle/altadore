#include "scene/transform_node.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace {

class TestSceneNode : public SceneNode {
 public:
  TestSceneNode(bool intersection) : intersection_(intersection) {
  }

  void CalculateTransforms(const Matrix4& parent_transform) {
    transform_ = parent_transform;
  }

  bool FindIntersection(const Ray& ray, double* t, Point3* point,
                        Vector3* normal, const Material** material) const {
    *material = reinterpret_cast<const Material*>(1);
    *t = t_val--;
    return intersection_;
  }

  bool HasIntersection(const Ray& ray, double max_t) const {
    return intersection_;
  }

  const Matrix4& transform() { return transform_; }

 private:
  static double t_val;

  Matrix4 transform_;
  bool intersection_;
};

double TestSceneNode::t_val = 10;

class TestTransformNode : public TransformNode {
 public:
  using TransformNode::children;
  using TransformNode::transform;
};

}  // namespace

TEST(TransformNodeTest, AddChild) {
  TestTransformNode node;
  node.AddChild(std::make_shared<TransformNode>());
  EXPECT_EQ(1, static_cast<int>(node.children().size()));
}

TEST(TransformNodeTest, Transform) {
  TestTransformNode node;
  node.Rotate(Matrix4::AXIS_X, 34);
  node.Scale(4);
  node.Scale(1, 2, 3);
  node.Translate(1, 2, 3);

  Matrix4 expected = Matrix4::GetTranslation(1, 2, 3) *
                     Matrix4::GetScaling(1, 2, 3) *
                     Matrix4::GetScaling(4) *
                     Matrix4::GetRotation(Matrix4::AXIS_X, 34);
  EXPECT_TRUE(node.transform() == expected);
}

TEST(TransformNodeTest, CalculateTransforms) {
  TestTransformNode node;
  node.AddChild(std::make_shared<TestSceneNode>(false));
  node.AddChild(std::make_shared<TestSceneNode>(false));
  node.AddChild(std::make_shared<TestSceneNode>(false));
  node.CalculateTransforms(Matrix4::GetScaling(5));

  for (const auto& child : node.children()) {
    TestSceneNode* test_child = dynamic_cast<TestSceneNode*>(child.get());
    EXPECT_NE(nullptr, test_child);
    EXPECT_TRUE(test_child->transform() == Matrix4::GetScaling(5));
  }
}

TEST(TransformNodeTest, FindIntersection) {
  double t;
  Point3 point;
  Vector3 normal;
  const Material* material;
  Ray ray(point, Vector3());

  TransformNode node;
  node.AddChild(std::make_shared<TestSceneNode>(false));
  EXPECT_FALSE(node.FindIntersection(ray, &t, &point, &normal, &material));

  node.AddChild(std::make_shared<TestSceneNode>(true));
  EXPECT_TRUE(node.FindIntersection(ray, &t, &point, &normal, &material));
  EXPECT_EQ(8, t);

  node.AddChild(std::make_shared<TestSceneNode>(true));
  EXPECT_TRUE(node.FindIntersection(ray, &t, &point, &normal, &material));
  EXPECT_EQ(5, t);
}

TEST(TransformNodeTest, HasIntersection) {
  Point3 p;
  Ray ray(p, Vector3());

  TransformNode node;
  node.AddChild(std::make_shared<TestSceneNode>(false));
  EXPECT_FALSE(node.HasIntersection(ray, 0));

  node.AddChild(std::make_shared<TestSceneNode>(true));
  EXPECT_TRUE(node.HasIntersection(ray, 0));
}

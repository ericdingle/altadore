#include "altadore/scene/transform_node.h"

#include "bonavista/testing/test_case.h"

namespace {

class TestSceneNode : public SceneNode {
public:
  TestSceneNode(bool intersection) : intersection_(intersection) {
  }

  void CalculateTransforms(const Matrix4& parent_transform) {
    transform_ = parent_transform;
  }
  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const {
    *material = reinterpret_cast<const Material*>(1);
    *t = t_val--;
    return intersection_;
  }
  bool HasIntersection(const Ray& ray) const {
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

TEST_CASE(TransformNodeTest) {
};

TEST(TransformNodeTest, AddChild) {
  TestTransformNode node;
  node.AddChild(new TransformNode());
  EXPECT_EQ(node.children().size(), 1);
}

TEST(TransformNodeTest, Transform) {
  TestTransformNode node;
  node.Rotate('x', 34);
  node.Scale(4);
  node.Scale(1, 2, 3);
  node.Translate(1, 2, 3);

  Matrix4 expected = Matrix4::GetTranslation(1, 2, 3) *
                     Matrix4::GetScaling(1, 2, 3) *
                     Matrix4::GetScaling(4) *
                     Matrix4::GetRotation('x', 34);
  EXPECT_TRUE(node.transform() == expected);
}

TEST(TransformNodeTest, CalculateTransforms) {
  TestTransformNode node;
  node.AddChild(new TestSceneNode(false));
  node.AddChild(new TestSceneNode(false));
  node.AddChild(new TestSceneNode(false));
  node.CalculateTransforms(Matrix4::GetScaling(5));

  for (uint i = 0; i < node.children().size(); ++i) {
    TestSceneNode* child = dynamic_cast<TestSceneNode*>(node.children()[i].ptr());
    EXPECT_NOT_NULL(child);
    EXPECT_TRUE(child->transform() == Matrix4::GetScaling(5));
  }
}

TEST(TransformNodeTest, FindIntersection) {
  double t;
  Point3 point;
  Vector3 normal;
  const Material* material;
  Ray ray(point, Vector3());

  TransformNode node;
  node.AddChild(new TestSceneNode(false));
  EXPECT_FALSE(node.FindIntersection(ray, &t, &point, &normal, &material));

  node.AddChild(new TestSceneNode(true));
  EXPECT_TRUE(node.FindIntersection(ray, &t, &point, &normal, &material));
  EXPECT_EQ(t, 8);

  node.AddChild(new TestSceneNode(true));
  EXPECT_TRUE(node.FindIntersection(ray, &t, &point, &normal, &material));
  EXPECT_EQ(t, 5);
}

TEST(TransformNodeTest, HasIntersection) {
  Point3 p;
  Ray ray(p, Vector3());

  TransformNode node;
  node.AddChild(new TestSceneNode(false));
  EXPECT_FALSE(node.HasIntersection(ray));

  node.AddChild(new TestSceneNode(true));
  EXPECT_TRUE(node.HasIntersection(ray));
}

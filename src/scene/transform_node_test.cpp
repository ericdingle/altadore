#include "altadore/scene/transform_node.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

namespace {

class TestSceneNode : public SceneNode {
 public:
  TestSceneNode(bool intersection) : intersection_(intersection) {
  }

  Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var) {
    return RESULT_ERR_NAME;
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

TEST_CASE(TransformNodeTest) {
 protected:
  std::vector<std::shared_ptr<const Variant> > args_;
  std::shared_ptr<const Variant> var_;
  std::shared_ptr<Invokable> object_;
};

TEST(TransformNodeTest, Create) {
  EXPECT_EQ(Invokable::RESULT_OK, TransformNode::Create(args_, &object_));
  EXPECT_NOT_NULL(object_.get());
}

TEST(TransformNodeTest, CreateError) {
  args_.push_back(std::make_shared<Variant>(1.0));
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE,
            TransformNode::Create(args_, &object_));
}

TEST(TransformNodeTest, InvokeAddChild) {
  object_.reset(new TransformNode());
  args_.push_back(std::make_shared<Variant>(object_));

  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_OK, node.Invoke("AddChild", args_, &var_));

  std::shared_ptr<Invokable> obj;
  EXPECT_TRUE(var_->Get(&obj));
  EXPECT_EQ(object_.get(), obj.get());
}

TEST(TransformNodeTest, InvokeAddChildError) {
  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE,
            node.Invoke("AddChild", args_, &var_));

  args_.push_back(std::make_shared<Variant>(1.0));

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE,
            node.Invoke("AddChild", args_, &var_));
}

TEST(TransformNodeTest, InvokeRotate) {
  args_.push_back(std::make_shared<Variant>(0.0));
  args_.push_back(std::make_shared<Variant>(45.0));

  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_OK, node.Invoke("Rotate", args_, &var_));
}

TEST(TransformNodeTest, InvokeRotateError) {
  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE,
            node.Invoke("Rotate", args_, &var_));

  args_.push_back(std::make_shared<Variant>(1));
  args_.push_back(std::make_shared<Variant>(45.0));

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE,
            node.Invoke("Rotate", args_, &var_));

  args_[0] = std::make_shared<Variant>(3.0);

  EXPECT_EQ(Invokable::RESULT_ERR_FAIL, node.Invoke("Rotate", args_, &var_));
}

TEST(TransformNodeTest, InvokeScale) {
  args_.push_back(std::make_shared<Variant>(1.0));

  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_OK, node.Invoke("Scale", args_, &var_));

  args_.push_back(std::make_shared<Variant>(2.0));
  args_.push_back(std::make_shared<Variant>(3.0));

  EXPECT_EQ(Invokable::RESULT_OK, node.Invoke("Scale", args_, &var_));
}

TEST(TransformNodeTest, InvokeScaleError) {
  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE, node.Invoke("Scale", args_, &var_));

  args_.push_back(std::make_shared<Variant>(1));

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE, node.Invoke("Scale", args_, &var_));

  args_[0] = std::make_shared<Variant>(1.0);
  args_.push_back(std::make_shared<Variant>(2.0));
  args_.push_back(std::make_shared<Variant>(3));

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE, node.Invoke("Scale", args_, &var_));
}

TEST(TransformNodeTest, InvokeTranslate) {
  args_.push_back(std::make_shared<Variant>(1.0));
  args_.push_back(std::make_shared<Variant>(2.0));
  args_.push_back(std::make_shared<Variant>(3.0));

  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_OK, node.Invoke("Translate", args_, &var_));
}

TEST(TransformNodeTest, InvokeTranslateError) {
  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE,
            node.Invoke("Translate", args_, &var_));

  args_.push_back(std::make_shared<Variant>(1.0));
  args_.push_back(std::make_shared<Variant>(2.0));
  args_.push_back(std::make_shared<Variant>(3));

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE,
            node.Invoke("Translate", args_, &var_));
}

TEST(TransformNodeTest, InvokeError) {
  TransformNode node;
  EXPECT_EQ(Invokable::RESULT_ERR_NAME, node.Invoke("a", args_, &var_));
}

TEST(TransformNodeTest, AddChild) {
  TestTransformNode node;
  node.AddChild(std::make_shared<TransformNode>());
  EXPECT_EQ(1u, node.children().size());
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

  for (uint i = 0; i < node.children().size(); ++i) {
    TestSceneNode* child = dynamic_cast<TestSceneNode*>(node.children()[i].get());
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

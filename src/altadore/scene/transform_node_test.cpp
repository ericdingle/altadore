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
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var) {
    return RESULT_ERR_NAME;
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
 protected:
  std::vector<scoped_refptr<const Variant> > args_;
  scoped_refptr<const Variant> var_;
  scoped_refptr<Invokable> object_;
};

TEST(TransformNodeTest, Create) {
  EXPECT_EQ(TransformNode::Create(args_, object_.Receive()),
            Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object_.ptr());
}

TEST(TransformNodeTest, CreateError) {
  args_.push_back(new Variant(1.0));

  EXPECT_EQ(TransformNode::Create(args_, object_.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);
}

TEST(TransformNodeTest, InvokeAddChild) {
  object_.Reset(new TransformNode());
  args_.push_back(new Variant(object_.ptr()));

  TransformNode node;
  EXPECT_EQ(node.Invoke("AddChild", args_, var_.Receive()),
            Invokable::RESULT_OK);

  scoped_refptr<Invokable> obj;
  EXPECT_TRUE(var_->Get(obj.Receive()));
  EXPECT_EQ(obj.ptr(), object_.ptr());
}

TEST(TransformNodeTest, InvokeAddChildError) {
  TransformNode node;
  EXPECT_EQ(node.Invoke("AddChild", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  args_.push_back(new Variant(1.0));

  EXPECT_EQ(node.Invoke("AddChild", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(TransformNodeTest, InvokeRotate) {
  args_.push_back(new Variant(0.0));
  args_.push_back(new Variant(45.0));

  TransformNode node;
  EXPECT_EQ(node.Invoke("Rotate", args_, var_.Receive()), Invokable::RESULT_OK);
}

TEST(TransformNodeTest, InvokeRotateError) {
  TransformNode node;
  EXPECT_EQ(node.Invoke("Rotate", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  args_.push_back(new Variant(1));
  args_.push_back(new Variant(45.0));

  EXPECT_EQ(node.Invoke("Rotate", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);

  args_[0] = new Variant(3.0);

  EXPECT_EQ(node.Invoke("Rotate", args_, var_.Receive()),
            Invokable::RESULT_ERR_FAIL);
}

TEST(TransformNodeTest, InvokeScale) {
  args_.push_back(new Variant(1.0));

  TransformNode node;
  EXPECT_EQ(node.Invoke("Scale", args_, var_.Receive()), Invokable::RESULT_OK);

  args_.push_back(new Variant(2.0));
  args_.push_back(new Variant(3.0));

  EXPECT_EQ(node.Invoke("Scale", args_, var_.Receive()), Invokable::RESULT_OK);
}

TEST(TransformNodeTest, InvokeScaleError) {
  TransformNode node;
  EXPECT_EQ(node.Invoke("Scale", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  args_.push_back(new Variant(1));

  EXPECT_EQ(node.Invoke("Scale", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);

  args_[0] = new Variant(1.0);
  args_.push_back(new Variant(2.0));
  args_.push_back(new Variant(3));

  EXPECT_EQ(node.Invoke("Scale", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(TransformNodeTest, InvokeTranslate) {
  args_.push_back(new Variant(1.0));
  args_.push_back(new Variant(2.0));
  args_.push_back(new Variant(3.0));

  TransformNode node;
  EXPECT_EQ(node.Invoke("Translate", args_, var_.Receive()),
            Invokable::RESULT_OK);
}

TEST(TransformNodeTest, InvokeTranslateError) {
  TransformNode node;
  EXPECT_EQ(node.Invoke("Translate", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  args_.push_back(new Variant(1.0));
  args_.push_back(new Variant(2.0));
  args_.push_back(new Variant(3));

  EXPECT_EQ(node.Invoke("Translate", args_, var_.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(TransformNodeTest, InvokeError) {
  TransformNode node;
  EXPECT_EQ(node.Invoke("a", args_, var_.Receive()),
            Invokable::RESULT_ERR_NAME);
}

TEST(TransformNodeTest, AddChild) {
  TestTransformNode node;
  node.AddChild(new TransformNode());
  EXPECT_EQ(node.children().size(), 1);
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

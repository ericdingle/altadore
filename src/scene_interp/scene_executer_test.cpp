#include "scene_interp/scene_executer.h"

#include "algebra/point3.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_parser.h"
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "shader/color.h"
#include "shader/light.h"
#include "shader/material.h"
#include "shape/cube.h"
#include "shape/sphere.h"
#include "third_party/chaparral/src/executer/invokable.h"
#include "third_party/bonavista/src/lexer/token_stream.h"

class Object : public Invokable {
 public:
  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var) {
    var->reset(new Variant(5));
    return name == "pass" ? RESULT_OK : RESULT_ERR_NAME;
  }
};

TEST_CASE(SceneExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new SceneParser(stream_.get()));
    executer_.reset(new SceneExecuter(parser_.get()));
  }

  SceneLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<SceneExecuter> executer_;
  std::shared_ptr<const Variant> var_;
  std::shared_ptr<Invokable> object_;
};

TEST(SceneExecuterTest, Constants) {
  Init("AXIS_X;");

  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(Matrix4::AXIS_X, d);
}

TEST(SceneExecuterTest, ExecuteDotAccessor) {
  Init("obj.pass(1);");

  object_.reset(new Object());
  var_.reset(new Variant(object_));
  executer_->SetVar("obj", var_);

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(5, i);
}

TEST(SceneExecuterTest, ExecuteDotAccessorError) {
  Init("obj.pass(1);");

  var_.reset(new Variant(5));
  executer_->SetVar("obj", var_);

  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.pass(a);");

  object_.reset(new Object());
  var_.reset(new Variant(object_));
  executer_->SetVar("obj", var_);

  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.fail(1);");

  object_.reset(new Object());
  var_.reset(new Variant(object_));
  executer_->SetVar("obj", var_);

  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteAssignment) {
  Init("a=1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(1, d);
}

TEST(SceneExecuterTest, ExecuteAssignmentError) {
  Init("a=b;");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");

  var_.reset(new Variant(5));
  executer_->SetVar("a", var_);

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(5, i);
}

TEST(SceneExecuterTest, ExecuteIdentifierError) {
  Init("a;");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNew) {
  Init("new Color();");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Color*>(object_.get()));

  Init("new Cube();");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Cube*>(object_.get()));

  Init("new Light(new Point3(1.0, 2.0, 3.0), new Color(0.1, 0.2, 0.3));");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Light*>(object_.get()));

  Init("new Material(new Color(), 25.0, 1.0);");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Material*>(object_.get()));

  Init("new Point3();");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Point3*>(object_.get()));

  Init("new ShapeNode(new Cube(), new Material(new Color(), 25.0, 1.0));");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<ShapeNode*>(object_.get()));

  Init("new Sphere();");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<Sphere*>(object_.get()));

  Init("new TransformNode();");
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_TRUE(var_->Get(&object_));
  EXPECT_NOT_NULL(dynamic_cast<TransformNode*>(object_.get()));
}

TEST(SceneExecuterTest, ExecuteNewError) {
  Init("new Color(a);");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());

  Init("new Blah();");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNumber) {
  Init("1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(1, d);
}

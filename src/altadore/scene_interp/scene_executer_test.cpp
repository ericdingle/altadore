#include "altadore/scene_interp/scene_executer.h"

#include "altadore/algebra/point3.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene_interp/scene_parser.h"
#include "altadore/scene/shape_node.h"
#include "altadore/scene/transform_node.h"
#include "altadore/shader/color.h"
#include "altadore/shader/light.h"
#include "altadore/shader/material.h"
#include "altadore/shape/cube.h"
#include "altadore/shape/sphere.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/executer/invokable.h"
#include "chaparral/lexer/token_stream.h"

class Object : public Invokable {
 public:
  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var) {
    scoped_refptr<const Variant> var_ref(new Variant(5));
    *var = var_ref.Release();
    return name == "pass" ? RESULT_OK : RESULT_ERR_NAME;
  }
};

TEST_CASE(SceneExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new SceneParser(stream_.ptr()));
    executer_.Reset(new SceneExecuter(parser_.ptr()));
  }

  SceneLexer lexer_;
  scoped_ptr<TokenStream> stream_;
  scoped_ptr<Parser> parser_;
  scoped_ptr<SceneExecuter> executer_;
  scoped_refptr<const Variant> var_;
  scoped_refptr<Invokable> object_;
};

TEST(SceneExecuterTest, Constants) {
  Init("AXIS_X;");

  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(d, Matrix4::AXIS_X);
}

TEST(SceneExecuterTest, ExecuteDotAccessor) {
  Init("obj.pass(1);");

  object_.Reset(new Object());
  var_.Reset(new Variant(object_.ptr()));
  executer_->SetVar("obj", var_.ptr());

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(i, 5);
}

TEST(SceneExecuterTest, ExecuteDotAccessorError) {
  Init("obj.pass(1);");

  var_.Reset(new Variant(5));
  executer_->SetVar("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.pass(a);");

  object_.Reset(new Object());
  var_.Reset(new Variant(object_.ptr()));
  executer_->SetVar("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.fail(1);");

  object_.Reset(new Object());
  var_.Reset(new Variant(object_.ptr()));
  executer_->SetVar("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteAssignment) {
  Init("a=1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(d, 1);
}

TEST(SceneExecuterTest, ExecuteAssignmentError) {
  Init("a=b;");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");

  var_.Reset(new Variant(5));
  executer_->SetVar("a", var_.ptr());

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(i, 5);
}

TEST(SceneExecuterTest, ExecuteIdentifierError) {
  Init("a;");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNew) {
  Init("new Color();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Color*>(object_.ptr()));

  Init("new Cube();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Cube*>(object_.ptr()));

  Init("new Light(new Point3(1.0, 2.0, 3.0), new Color(0.1, 0.2, 0.3));");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Light*>(object_.ptr()));

  Init("new Material(new Color(), 25.0, 1.0);");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Material*>(object_.ptr()));

  Init("new Point3();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Point3*>(object_.ptr()));

  Init("new ShapeNode(new Cube(), new Material(new Color(), 25.0, 1.0));");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<ShapeNode*>(object_.ptr()));

  Init("new Sphere();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Sphere*>(object_.ptr()));

  Init("new TransformNode();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object_.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<TransformNode*>(object_.ptr()));
}

TEST(SceneExecuterTest, ExecuteNewError) {
  Init("new Color(a);");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("new Blah();");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNumber) {
  Init("1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(d, 1);
}

#include "algebra/matrix4.h"
#include "algebra/point3.h"
#include "scene_interp/scene_executer.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"
#include "scene_interp/scene_parser.h"
#include "shader/color.h"
#include "shader/light.h"
#include "shader/material.h"
#include "third_party/bonavista/src/util/status_test_macros.h"
#include "third_party/chaparral/src/executer/any_test_macros.h"
#include "third_party/chaparral/src/executer/executer_test_fixture.h"

class SceneExecuterTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(SceneExecuterTest, DefaultVariables) {
  EXPECT_ANY(Execute("AXIS_X;").value(), double, Matrix4::AXIS_X);
  EXPECT_ANY(Execute("AXIS_Y;").value(), double, Matrix4::AXIS_Y);
  EXPECT_ANY(Execute("AXIS_Z;").value(), double, Matrix4::AXIS_Z);
}

class TestSceneObject : public SceneObject {
 public:
  StatusOr<Any> Get(const std::string& name) override {
    return name == "one" ? Any(1) : Any(2);
  }
};

TEST_F(SceneExecuterTest, ExecuteDotAccessor) {
  Init("a.one; a.two;");
  executer_->SetVariable("a", Any(std::shared_ptr<SceneObject>(new TestSceneObject())));
  EXPECT_ANY(executer_->Execute().value(), int, 1);
  EXPECT_ANY(executer_->Execute().value(), int, 2);
}

TEST_F(SceneExecuterTest, ExecuteDotAccessorError) {
  EXPECT_STATUS(Execute("a.one;").status(), "a is undefined", 1, 1);
}

TEST_F(SceneExecuterTest, ExecuteAssignment) {
  EXPECT_ANY(Execute("a = 1;").value(), double, 1);
  EXPECT_ANY(executer_->GetVariable("a"), double, 1);
}

TEST_F(SceneExecuterTest, ExecuteAssignmentError) {
  EXPECT_STATUS(Execute("a = b;").status(), "b is undefined", 1, 5);
}

StatusOr<Any> Func(const std::vector<const Node*>& args, int line, int column) {
  if (!args.empty()) {
    return Any(1);
  }
  return Status("error", line, column);
}

TEST_F(SceneExecuterTest, ExecuteFunction) {
  Init("a(1);");
  SceneFunc func = &Func;
  executer_->SetVariable("a", Any(func));
  EXPECT_ANY(executer_->Execute().value(), int, 1);
}

TEST_F(SceneExecuterTest, ExecuteFunctionError) {
  EXPECT_STATUS(Execute("a();").status(), "a is undefined", 1, 1);

  Init("a();");
  SceneFunc func = &Func;
  executer_->SetVariable("a", Any(func));
  EXPECT_STATUS(executer_->Execute().status(), "error", 1, 2);
}

TEST_F(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");
  executer_->SetVariable("a", Any(5));
  EXPECT_ANY(executer_->Execute().value(), int, 5);
}

TEST_F(SceneExecuterTest, ExecuteIdentifierError) {
  EXPECT_STATUS(Execute("a;").status(), "a is undefined", 1, 1);
}

TEST_F(SceneExecuterTest, ExecuteNumber) {
  EXPECT_ANY(Execute("1;").value(), double, 1);
}

TEST_F(SceneExecuterTest, CreateColor) {
  std::shared_ptr<Color> color;
  EXPECT_TRUE(Execute("Color(0.1, 0.2, 0.3);").value().Get(&color));
  EXPECT_EQ(0.1, color->r());
  EXPECT_EQ(0.2, color->g());
  EXPECT_EQ(0.3, color->b());
}

TEST_F(SceneExecuterTest, CreateColorError) {
  EXPECT_STATUS(Execute("Color();").status(), "Expecting 3 argument(s)", 1, 6);
  EXPECT_STATUS(Execute("Color(a, 1, 2);").status(), "a is undefined", 1, 7);
  EXPECT_STATUS(Execute("Color(1, a, 2);").status(), "a is undefined", 1, 10);
  EXPECT_STATUS(Execute("Color(1, 2, a);").status(), "a is undefined", 1, 13);
}

TEST_F(SceneExecuterTest, CreateLight) {
  std::shared_ptr<Light> light;
  EXPECT_TRUE(Execute("Light(Point3(0, 0, 0), Color(0, 0, 0));").value().Get(&light));
}

TEST_F(SceneExecuterTest, CreateLightError) {
  EXPECT_STATUS(Execute("Light();").status(), "Expecting 2 argument(s)", 1, 6);
  EXPECT_STATUS(Execute("Light(a, Color(0, 0, 0));").status(), "a is undefined", 1, 7);
  EXPECT_STATUS(Execute("Light(Point3(0, 0, 0), a);").status(), "a is undefined", 1, 24);
}

TEST_F(SceneExecuterTest, CreatePoint3) {
  std::shared_ptr<Point3> point3;
  EXPECT_TRUE(Execute("Point3(0.1, 0.2, 0.3);").value().Get(&point3));
  EXPECT_EQ(0.1, (*point3)[0]);
  EXPECT_EQ(0.2, (*point3)[1]);
  EXPECT_EQ(0.3, (*point3)[2]);
}

TEST_F(SceneExecuterTest, CreatePoint3Error) {
  EXPECT_STATUS(Execute("Point3();").status(), "Expecting 3 argument(s)", 1, 7);
  EXPECT_STATUS(Execute("Point3(a, 1, 2);").status(), "a is undefined", 1, 8);
  EXPECT_STATUS(Execute("Point3(1, a, 2);").status(), "a is undefined", 1, 11);
  EXPECT_STATUS(Execute("Point3(1, 2, a);").status(), "a is undefined", 1, 14);
}

TEST_F(SceneExecuterTest, CreateMaterial) {
  std::shared_ptr<Material> material;
  EXPECT_TRUE(Execute("Material(Color(0, 0, 0), 0.1, 0.2);").value().Get(&material));
  EXPECT_EQ(0.1, material->shininess());
  EXPECT_EQ(0.2, material->reflectivity());
}

TEST_F(SceneExecuterTest, CreateMaterialError) {
  EXPECT_STATUS(Execute("Material();").status(), "Expecting 3 argument(s)", 1, 9);
  EXPECT_STATUS(Execute("Material(a, 1, 2);").status(), "a is undefined", 1, 10);
  EXPECT_STATUS(Execute("Material(Color(0, 0, 0), a, 2);").status(), "a is undefined", 1, 26);
  EXPECT_STATUS(Execute("Material(Color(0, 0, 0), 1, a);").status(), "a is undefined", 1, 29);
}


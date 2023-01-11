#include "altadore/algebra/matrix4.h"
#include "altadore/algebra/point3.h"
#include "altadore/scene/transform_node.h"
#include "altadore/scene_interp/scene_executer.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene_interp/scene_object.h"
#include "altadore/scene_interp/scene_parser.h"
#include "altadore/shader/color.h"
#include "altadore/shader/light.h"
#include "altadore/shader/material.h"
#include "altadore/shape/cube.h"
#include "altadore/shape/sphere.h"
#include "bonavista/executer/executer_test_fixture.h"
#include "bonavista/util/error.h"

class SceneExecuterTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(SceneExecuterTest, DefaultVariables) {
  EXPECT_EQ(Matrix4::AXIS_X, std::any_cast<double>(Execute("AXIS_X;").value()));
  EXPECT_EQ(Matrix4::AXIS_Y, std::any_cast<double>(Execute("AXIS_Y;").value()));
  EXPECT_EQ(Matrix4::AXIS_Z, std::any_cast<double>(Execute("AXIS_Z;").value()));

  auto obj = std::any_cast<std::shared_ptr<SceneObject>>(Execute("root;").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<TransformNode>(obj));

  obj = std::any_cast<std::shared_ptr<SceneObject>>(Execute("lights;").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<std::vector<std::shared_ptr<Light>>>(obj));
}

class TestSceneObject : public SceneObject {
 public:
  TestSceneObject() : SceneObject(nullptr) {}

  absl::StatusOr<std::any> Get(const Token& token) override {
    return std::any(token.value() == "one" ? 1 : 2);
  }
};

TEST_F(SceneExecuterTest, ExecuteDotAccessor) {
  Init("a.one; a.two;");
  executer_->SetVariable("a", std::any(std::shared_ptr<SceneObject>(
      new TestSceneObject())));
  EXPECT_EQ(1, std::any_cast<int>(executer_->Execute().value()));
  EXPECT_EQ(2, std::any_cast<int>(executer_->Execute().value()));
}

TEST_F(SceneExecuterTest, ExecuteDotAccessorError) {
  EXPECT_EQ(Execute("a.one;").status().message(), "'a' is undefined at 1:1.");
}

TEST_F(SceneExecuterTest, ExecuteAssignment) {
  EXPECT_EQ(1, std::any_cast<double>(Execute("a = 1;").value()));
  EXPECT_EQ(1, std::any_cast<double>(executer_->GetVariable("a")));
}

TEST_F(SceneExecuterTest, ExecuteAssignmentError) {
  EXPECT_EQ(Execute("a = b;").status().message(), "'b' is undefined at 1:5.");
}

absl::StatusOr<std::any> Func(const Token& token, const std::vector<const Node*>& args) {
  if (!args.empty()) {
    return std::any(1);
  }
  return Error("error", token.line(), token.column());
}

TEST_F(SceneExecuterTest, ExecuteFunction) {
  Init("a(1);");
  SceneFunc func = &Func;
  executer_->SetVariable("a", std::any(func));
  EXPECT_EQ(1, std::any_cast<int>(executer_->Execute().value()));
}

TEST_F(SceneExecuterTest, ExecuteFunctionError) {
  EXPECT_EQ(Execute("a();").status().message(), "'a' is undefined at 1:1.");

  Init("a();");
  SceneFunc func = &Func;
  executer_->SetVariable("a", std::any(func));
  EXPECT_EQ(executer_->Execute().status().message(), "error at 1:2.");
}

TEST_F(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");
  executer_->SetVariable("a", std::any(5));
  EXPECT_EQ(5, std::any_cast<int>(executer_->Execute().value()));
}

TEST_F(SceneExecuterTest, ExecuteIdentifierError) {
  EXPECT_EQ(Execute("a;").status().message(), "'a' is undefined at 1:1.");
}

TEST_F(SceneExecuterTest, ExecuteNumber) {
  EXPECT_EQ(1, std::any_cast<double>(Execute("1;").value()));
}

TEST_F(SceneExecuterTest, CreateColor) {
  auto color = std::any_cast<std::shared_ptr<Color>>(
      Execute("Color(0.1, 0.2, 0.3);").value());
  EXPECT_EQ(0.1, color->r());
  EXPECT_EQ(0.2, color->g());
  EXPECT_EQ(0.3, color->b());
}

TEST_F(SceneExecuterTest, CreateColorError) {
  EXPECT_EQ(Execute("Color();").status().message(),
            "Expecting 3 argument(s) at 1:6.");
  EXPECT_EQ(Execute("Color(a, 1, 2);").status().message(),
            "'a' is undefined at 1:7.");
  EXPECT_EQ(Execute("Color(1, a, 2);").status().message(),
            "'a' is undefined at 1:10.");
  EXPECT_EQ(Execute("Color(1, 2, a);").status().message(),
            "'a' is undefined at 1:13.");
}

TEST_F(SceneExecuterTest, CreateCube) {
  auto obj = std::any_cast<std::shared_ptr<Shape>>(Execute("Cube();").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<Cube>(obj));
}

TEST_F(SceneExecuterTest, CreateCubeError) {
  EXPECT_EQ(Execute("Cube(1);").status().message(),
            "Expecting 0 argument(s) at 1:5.");
}

TEST_F(SceneExecuterTest, CreateLight) {
  auto light = std::any_cast<std::shared_ptr<Light>>(
      Execute("Light(Point3(0, 0, 0), Color(0, 0, 0));").value());
}

TEST_F(SceneExecuterTest, CreateLightError) {
  EXPECT_EQ(Execute("Light();").status().message(),
            "Expecting 2 argument(s) at 1:6.");
  EXPECT_EQ(Execute("Light(a, Color(0, 0, 0));").status().message(),
            "'a' is undefined at 1:7.");
  EXPECT_EQ(Execute("Light(Point3(0, 0, 0), a);").status().message(),
            "'a' is undefined at 1:24.");
}

TEST_F(SceneExecuterTest, CreateMaterial) {
  auto material = std::any_cast<std::shared_ptr<Material>>(
      Execute("Material(Color(0, 0, 0), 0.1, 0.2);").value());
  EXPECT_EQ(0.1, material->shininess());
  EXPECT_EQ(0.2, material->reflectivity());
}

TEST_F(SceneExecuterTest, CreateMaterialError) {
  EXPECT_EQ(Execute("Material();").status().message(),
            "Expecting 3 argument(s) at 1:9.");
  EXPECT_EQ(Execute("Material(a, 1, 2);").status().message(),
            "'a' is undefined at 1:10.");
  EXPECT_EQ(Execute("Material(Color(0, 0, 0), a, 2);").status().message(),
            "'a' is undefined at 1:26.");
  EXPECT_EQ(Execute("Material(Color(0, 0, 0), 1, a);").status().message(),
            "'a' is undefined at 1:29.");
}

TEST_F(SceneExecuterTest, CreatePoint3) {
  auto point3 = std::any_cast<std::shared_ptr<Point3>>(
      Execute("Point3(0.1, 0.2, 0.3);").value());
  EXPECT_EQ(0.1, (*point3)[0]);
  EXPECT_EQ(0.2, (*point3)[1]);
  EXPECT_EQ(0.3, (*point3)[2]);
}

TEST_F(SceneExecuterTest, CreatePoint3Error) {
  EXPECT_EQ(Execute("Point3();").status().message(),
            "Expecting 3 argument(s) at 1:7.");
  EXPECT_EQ(Execute("Point3(a, 1, 2);").status().message(),
            "'a' is undefined at 1:8.");
  EXPECT_EQ(Execute("Point3(1, a, 2);").status().message(),
            "'a' is undefined at 1:11.");
  EXPECT_EQ(Execute("Point3(1, 2, a);").status().message(),
            "'a' is undefined at 1:14.");
}

TEST_F(SceneExecuterTest, CreateShapeNode) {
  auto obj = std::any_cast<std::shared_ptr<SceneObject>>(
      Execute("ShapeNode(Cube(), Material(Color(0, 0, 0), 1, 1));").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<ShapeNode>(obj));
}

TEST_F(SceneExecuterTest, CreateShapeNodeError) {
  EXPECT_EQ(Execute("ShapeNode();").status().message(),
            "Expecting 2 argument(s) at 1:10.");
  EXPECT_EQ(Execute("ShapeNode(a, Material(Color(0, 0, 0), 1, 1));").status().message(),
            "'a' is undefined at 1:11.");
  EXPECT_EQ(Execute("ShapeNode(Cube(), a);").status().message(),
            "'a' is undefined at 1:19.");
}

TEST_F(SceneExecuterTest, CreateSphere) {
  auto obj = std::any_cast<std::shared_ptr<Shape>>(Execute("Sphere();").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<Sphere>(obj));
}

TEST_F(SceneExecuterTest, CreateSphereError) {
  EXPECT_EQ(Execute("Sphere(1);").status().message(),
            "Expecting 0 argument(s) at 1:7.");
}

TEST_F(SceneExecuterTest, CreateTransformNode) {
  auto obj = std::any_cast<std::shared_ptr<SceneObject>>(
      Execute("TransformNode();").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<TransformNode>(obj));
}

TEST_F(SceneExecuterTest, CreateTransformNodeError) {
  EXPECT_EQ(Execute("TransformNode(1);").status().message(),
            "Expecting 0 argument(s) at 1:14.");
}

#include "algebra/matrix4.h"
#include "scene_interp/scene_executer.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"
#include "scene_interp/scene_parser.h"
#include "third_party/bonavista/src/util/status_test_macros.h"
#include "third_party/chaparral/src/executer/any_test_macros.h"
#include "third_party/chaparral/src/executer/executer_test_fixture.h"

class SceneExecuterTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(SceneExecuterTest, Constants) {
  EXPECT_ANY(Execute("AXIS_X;").value(), double, Matrix4::AXIS_X);
  EXPECT_ANY(Execute("AXIS_Y;").value(), double, Matrix4::AXIS_Y);
  EXPECT_ANY(Execute("AXIS_Z;").value(), double, Matrix4::AXIS_Z);
}

class TestSceneObject : public SceneObject {
 public:
  StatusOr<Any> Get(const std::string& name) {
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

StatusOr<Any> Func(const std::vector<const Node*>& args) {
  if (args.empty()) {
    return Any(5);
  }
  return Status("error", args[0]->token().line(), args[0]->token().column());
}

TEST_F(SceneExecuterTest, ExecuteFunction) {
  Init("a();");
  SceneFunc func = &Func;
  executer_->SetVariable("a", Any(func));
  EXPECT_ANY(executer_->Execute().value(), int, 5);
}

TEST_F(SceneExecuterTest, ExecuteFunctionError) {
  EXPECT_STATUS(Execute("a();").status(), "a is undefined", 1, 1);

  Init("a(1);");
  SceneFunc func = &Func;
  executer_->SetVariable("a", Any(func));
  EXPECT_STATUS(executer_->Execute().status(), "error", 1, 3);
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

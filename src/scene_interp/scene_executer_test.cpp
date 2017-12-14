#include "scene_interp/scene_executer.h"

#include "algebra/matrix4.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_parser.h"
#include "third_party/bonavista/src/util/status_test_macros.h"
#include "third_party/chaparral/src/executer/any_test_macros.h"
#include "third_party/chaparral/src/executer/executer_test_fixture.h"

class SceneExecuter2 : public SceneExecuter {
 public:
  using SceneExecuter::SceneExecuter;

  StatusOr<std::shared_ptr<Any>> Func(const std::vector<const Node*>& args) {
    if (args.empty()) {
      return std::make_shared<Any>(5);
    }
    return Status("error", args[0]->token().line(), args[0]->token().column());
  }
};

class SceneExecuterTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter2> {
};

TEST_F(SceneExecuterTest, Constants) {
  EXPECT_ANY(Execute("AXIS_X;").value(), double, Matrix4::AXIS_X);
  EXPECT_ANY(Execute("AXIS_Y;").value(), double, Matrix4::AXIS_Y);
  EXPECT_ANY(Execute("AXIS_Z;").value(), double, Matrix4::AXIS_Z);
}

/*
TEST_F(SceneExecuterTest, ExecuteDotAccessor) {
}

TEST_F(SceneExecuterTest, ExecuteDotAccessorError) {
}
*/

TEST_F(SceneExecuterTest, ExecuteAssignment) {
  EXPECT_ANY(Execute("a = 1;").value(), double, 1);
  EXPECT_ANY(executer_->GetVariable("a"), double, 1);
}

TEST_F(SceneExecuterTest, ExecuteAssignmentError) {
  EXPECT_STATUS(Execute("a = b;").status(), "b is undefined", 1, 5);
}

TEST_F(SceneExecuterTest, ExecuteFunction) {
  Init("a();");
  SceneFunc func = std::bind(&SceneExecuter2::Func, executer_.get(),
                             std::placeholders::_1);
  executer_->SetVariable("a", std::make_shared<Any>(func));
  EXPECT_ANY(executer_->Execute().value(), int, 5);
}

TEST_F(SceneExecuterTest, ExecuteFunctionError) {
  EXPECT_STATUS(Execute("a();").status(), "a is undefined", 1, 1);

  Init("a(1);");
  SceneFunc func = std::bind(&SceneExecuter2::Func, executer_.get(),
                             std::placeholders::_1);
  executer_->SetVariable("a", std::make_shared<Any>(func));
  EXPECT_STATUS(executer_->Execute().status(), "error", 1, 3);
}

TEST_F(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");
  executer_->SetVariable("a", std::make_shared<Any>(5));
  EXPECT_ANY(executer_->Execute().value(), int, 5);
}

TEST_F(SceneExecuterTest, ExecuteIdentifierError) {
  EXPECT_STATUS(Execute("a;").status(), "a is undefined", 1, 1);
}

TEST_F(SceneExecuterTest, ExecuteNumber) {
  EXPECT_ANY(Execute("1;").value(), double, 1);
}

#include "scene_interp/scene_executer.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"
#include "scene_interp/scene_parser.h"
#include "third_party/bonavista/src/util/status_test_macros.h"
#include "third_party/chaparral/src/executer/any_test_macros.h"
#include "third_party/chaparral/src/executer/executer_test_fixture.h"

class TransformNodeObjectTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(TransformNodeObjectTest, Undefined) {
  EXPECT_STATUS(Execute("TransformNode().method();").status(), "method is undefined", 1, 17);
}

TEST_F(TransformNodeObjectTest, AddChild) {
  std::shared_ptr<SceneObject> node;
  EXPECT_TRUE(Execute("TransformNode().AddChild(TransformNode());").value().Get(&node));
  EXPECT_TRUE(std::dynamic_pointer_cast<TransformNode>(node));
}

TEST_F(TransformNodeObjectTest, AddChildError) {
  EXPECT_STATUS(Execute("TransformNode().AddChild();").status(),
                "Expecting 1 argument(s)", 1, 25);
  EXPECT_STATUS(Execute("TransformNode().AddChild(Cube());").status(),
                "Expected type: St10shared_ptrI11SceneObjectE", 1, 30);
  // TODO: test if arg is SceneObject but not SceneNode.
}

TEST_F(TransformNodeObjectTest, Rotate) {
  EXPECT_OK(Execute("TransformNode().Rotate(AXIS_X, 30);"));
}

TEST_F(TransformNodeObjectTest, RotateError) {
  EXPECT_STATUS(Execute("TransformNode().Rotate();").status(),
                "Expecting 2 argument(s)", 1, 23);
  EXPECT_STATUS(Execute("TransformNode().Rotate(Cube, 30);").status(),
                "Expected type: d", 1, 24);
  EXPECT_STATUS(Execute("TransformNode().Rotate(5, 30);").status(),
                "Expected type: Matrix4::AXIS", 1, 24);
  EXPECT_STATUS(Execute("TransformNode().Rotate(AXIS_X, Cube);").status(),
                "Expected type: d", 1, 32);
}

TEST_F(TransformNodeObjectTest, Scale) {
  EXPECT_OK(Execute("TransformNode().Scale(1, 2, 3);"));
}

TEST_F(TransformNodeObjectTest, ScaleError) {
  EXPECT_STATUS(Execute("TransformNode().Scale();").status(),
                "Expecting 3 argument(s)", 1, 22);
  EXPECT_STATUS(Execute("TransformNode().Scale(Cube, 1, 2);").status(),
                "Expected type: d", 1, 23);
  EXPECT_STATUS(Execute("TransformNode().Scale(1, Cube, 2);").status(),
                "Expected type: d", 1, 26);
  EXPECT_STATUS(Execute("TransformNode().Scale(1, 2, Cube);").status(),
                "Expected type: d", 1, 29);
}

TEST_F(TransformNodeObjectTest, Translate) {
  EXPECT_OK(Execute("TransformNode().Translate(1, 2, 3);"));
}

TEST_F(TransformNodeObjectTest, TranslateError) {
  EXPECT_STATUS(Execute("TransformNode().Translate();").status(),
                "Expecting 3 argument(s)", 1, 26);
  EXPECT_STATUS(Execute("TransformNode().Translate(Cube, 1, 2);").status(),
                "Expected type: d", 1, 27);
  EXPECT_STATUS(Execute("TransformNode().Translate(1, Cube, 2);").status(),
                "Expected type: d", 1, 30);
  EXPECT_STATUS(Execute("TransformNode().Translate(1, 2, Cube);").status(),
                "Expected type: d", 1, 33);
}


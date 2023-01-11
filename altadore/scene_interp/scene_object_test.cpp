#include "altadore/scene_interp/scene_object.h"

#include "altadore/scene_interp/scene_executer.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene_interp/scene_parser.h"
#include "bonavista/executer/executer_test_fixture.h"

class TransformNodeObjectTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(TransformNodeObjectTest, Constructor) {
  auto node = std::any_cast<std::shared_ptr<SceneObject>>(
      Execute("TransformNode();").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<TransformNode>(node));
}

TEST_F(TransformNodeObjectTest, Undefined) {
  EXPECT_EQ(Execute("TransformNode().method();").status().message(),
            "'method' is undefined at 1:17.");
}

TEST_F(TransformNodeObjectTest, AddChild) {
  auto node = std::any_cast<std::shared_ptr<SceneObject>>(
      Execute("TransformNode().AddChild(TransformNode());").value());
  EXPECT_TRUE(std::dynamic_pointer_cast<TransformNode>(node));
}

TEST_F(TransformNodeObjectTest, AddChildError) {
  EXPECT_EQ(Execute("TransformNode().AddChild();").status().message(),
            "Expecting 1 argument(s) at 1:25.");
  EXPECT_EQ(Execute("TransformNode().AddChild(Cube());").status().message(),
            "Expected type 'St10shared_ptrI11SceneObjectE' at 1:30.");
  EXPECT_EQ(Execute("TransformNode().AddChild(lights);").status().message(),
            "Expected type 'SceneNode' at 1:26.");
}

TEST_F(TransformNodeObjectTest, Rotate) {
  EXPECT_OK(Execute("TransformNode().Rotate(AXIS_X, 30);").status());
}

TEST_F(TransformNodeObjectTest, RotateError) {
  EXPECT_EQ(Execute("TransformNode().Rotate();").status().message(),
            "Expecting 2 argument(s) at 1:23.");
  EXPECT_EQ(Execute("TransformNode().Rotate(Cube, 30);").status().message(),
            "Expected type 'd' at 1:24.");
  EXPECT_EQ(Execute("TransformNode().Rotate(5, 30);").status().message(),
            "Expected type 'Matrix4::AXIS' at 1:24.");
  EXPECT_EQ(Execute("TransformNode().Rotate(AXIS_X, Cube);").status().message(),
            "Expected type 'd' at 1:32.");
}

TEST_F(TransformNodeObjectTest, Scale) {
  EXPECT_OK(Execute("TransformNode().Scale(1, 2, 3);").status());
}

TEST_F(TransformNodeObjectTest, ScaleError) {
  EXPECT_EQ(Execute("TransformNode().Scale();").status().message(),
            "Expecting 3 argument(s) at 1:22.");
  EXPECT_EQ(Execute("TransformNode().Scale(Cube, 1, 2);").status().message(),
            "Expected type 'd' at 1:23.");
  EXPECT_EQ(Execute("TransformNode().Scale(1, Cube, 2);").status().message(),
            "Expected type 'd' at 1:26.");
  EXPECT_EQ(Execute("TransformNode().Scale(1, 2, Cube);").status().message(),
            "Expected type 'd' at 1:29.");
}

TEST_F(TransformNodeObjectTest, Translate) {
  EXPECT_OK(Execute("TransformNode().Translate(1, 2, 3);").status());
}

TEST_F(TransformNodeObjectTest, TranslateError) {
  EXPECT_EQ(Execute("TransformNode().Translate();").status().message(),
            "Expecting 3 argument(s) at 1:26.");
  EXPECT_EQ(Execute("TransformNode().Translate(Cube, 1, 2);").status().message(),
            "Expected type 'd' at 1:27.");
  EXPECT_EQ(Execute("TransformNode().Translate(1, Cube, 2);").status().message(),
            "Expected type 'd' at 1:30.");
  EXPECT_EQ(Execute("TransformNode().Translate(1, 2, Cube);").status().message(),
            "Expected type 'd' at 1:33.");
}

class LightVectorTest
    : public ExecuterTestFixture<SceneLexer, SceneParser, SceneExecuter> {
};

TEST_F(LightVectorTest, Undefined) {
  EXPECT_EQ(Execute("lights.method();").status().message(),
            "'method' is undefined at 1:8.");
}

TEST_F(LightVectorTest, AddLight) {
  auto light = std::any_cast<std::shared_ptr<Light>>(
      Execute("lights.AddLight(Light(Point3(0, 0, 0), Color(0, 0, 0)));").value());
}

TEST_F(LightVectorTest, AddLightError) {
  EXPECT_EQ(Execute("lights.AddLight();").status().message(),
            "Expecting 1 argument(s) at 1:16.");
  EXPECT_EQ(Execute("lights.AddLight(Cube());").status().message(),
            "Expected type 'St10shared_ptrI5LightE' at 1:21.");
}

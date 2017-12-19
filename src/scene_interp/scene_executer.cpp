#include "scene_interp/scene_executer.h"

#include <algorithm>
#include <assert.h>
#include "algebra/matrix4.h"
#include "algebra/point3.h"
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"
#include "shader/color.h"
#include "shader/light.h"
#include "shader/material.h"
#include "shape/cube.h"
#include "shape/sphere.h"

using namespace std::placeholders;

SceneExecuter::SceneExecuter(Parser* parser) : Executer(parser) {
  SetVariable("AXIS_X", Any(static_cast<double>(Matrix4::AXIS_X)));
  SetVariable("AXIS_Y", Any(static_cast<double>(Matrix4::AXIS_Y)));
  SetVariable("AXIS_Z", Any(static_cast<double>(Matrix4::AXIS_Z)));

  SetVariable("Color", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateColor, this, _1, _2, _3))));
  SetVariable("Cube", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateCube, this, _1, _2, _3))));
  SetVariable("Light", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateLight, this, _1, _2, _3))));
  SetVariable("Material", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateMaterial, this, _1, _2, _3))));
  SetVariable("Point3", Any(SceneFunc(std::bind(
      &SceneExecuter::CreatePoint3, this, _1, _2, _3))));
  SetVariable("ShapeNode", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateShapeNode, this, _1, _2, _3))));
  SetVariable("Sphere", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateSphere, this, _1, _2, _3))));
  SetVariable("TransformNode", Any(SceneFunc(std::bind(
      &SceneExecuter::CreateTransformNode, this, _1, _2, _3))));
}

Any SceneExecuter::GetVariable(const std::string& name) const {
  auto it = variables_.find(name);
  return it == variables_.end() ? Any() : it->second;
}

void SceneExecuter::SetVariable(const std::string& name, const Any& any) {
  variables_[name] = any;
}

StatusOr<Any> SceneExecuter::ExecuteNode(const Node* node) {
  switch (node->token().type()) {
    case SceneLexer::TYPE_EQUAL:
      return ExecuteAssignment(node);
    case SceneLexer::TYPE_DOT:
      return ExecuteDotAccessor(node);
    case SceneLexer::TYPE_LEFT_PARENTHESIS:
      return ExecuteFunction(node);
    case SceneLexer::TYPE_IDENTIFIER:
      return ExecuteIdentifier(node);
    case SceneLexer::TYPE_NUMBER:
      return ExecuteNumber(node);
    default:
      assert(false);
      return Status("WTF!", 1, 1);
  }
}

StatusOr<Any> SceneExecuter::ExecuteAssignment(const Node* node) {
  const std::string& name = node->children()[0]->token().value();
  ASSIGN_OR_RETURN(auto value, ExecuteNode(node->children()[1].get()));
  variables_[name] = value;
  return value;
}

StatusOr<Any> SceneExecuter::ExecuteDotAccessor(const Node* node) {
  ASSIGN_OR_RETURN(auto obj, ExecuteNodeT<std::shared_ptr<SceneObject>>(node->children()[0].get()));
  return obj->Get(node->children()[1]->token().value());
}

StatusOr<Any> SceneExecuter::ExecuteFunction(const Node* node) {
  const auto& children = node->children();
  ASSIGN_OR_RETURN(auto func, ExecuteNodeT<SceneFunc>(children[0].get()));

  std::vector<const Node*> args;
  std::transform(children.begin() + 1, children.end(), std::back_inserter(args),
                 [](const std::unique_ptr<const Node>& n) { return n.get(); });

  return func(args, node->token().line(), node->token().column());
}

StatusOr<Any> SceneExecuter::ExecuteIdentifier(const Node* node) {
  const std::string& name = node->token().value();

  if (variables_.count(name) == 0) {
    return Status(name + " is undefined", node->token().line(),
                  node->token().column());
  }

  return variables_[name];
}

StatusOr<Any> SceneExecuter::ExecuteNumber(const Node* node) {
  double value = atof(node->token().value().c_str());
  return Any(value);
}

Status SceneExecuter::ExpectSize(const std::vector<const Node*>& args, int size,
                                 int line, int column) {
  return args.size() == size ? Status() : Status(
      "Expecting " + std::to_string(size) + " argument(s)", line, column);
}

StatusOr<Any> SceneExecuter::CreateColor(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 3, line, column));
  ASSIGN_OR_RETURN(double r, ExecuteNodeT<double>(args[0]));
  ASSIGN_OR_RETURN(double g, ExecuteNodeT<double>(args[1]));
  ASSIGN_OR_RETURN(double b, ExecuteNodeT<double>(args[2]));
  return Any(std::make_shared<Color>(r, g, b));
};

StatusOr<Any> SceneExecuter::CreateCube(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 0, line, column));
  return Any(std::shared_ptr<SceneObject>(new CubeObject()));
};

StatusOr<Any> SceneExecuter::CreateLight(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 2, line, column));
  ASSIGN_OR_RETURN(auto p, ExecuteNodeT<std::shared_ptr<Point3>>(args[0]));
  ASSIGN_OR_RETURN(auto c, ExecuteNodeT<std::shared_ptr<Color>>(args[1]));
  return Any(std::make_shared<Light>(p, c));
};

StatusOr<Any> SceneExecuter::CreateMaterial(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 3, line, column));
  ASSIGN_OR_RETURN(auto c, ExecuteNodeT<std::shared_ptr<Color>>(args[0]));
  ASSIGN_OR_RETURN(double s, ExecuteNodeT<double>(args[1]));
  ASSIGN_OR_RETURN(double r, ExecuteNodeT<double>(args[2]));
  return Any(std::make_shared<Material>(c, s, r));
};

StatusOr<Any> SceneExecuter::CreatePoint3(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 3, line, column));
  ASSIGN_OR_RETURN(double x, ExecuteNodeT<double>(args[0]));
  ASSIGN_OR_RETURN(double y, ExecuteNodeT<double>(args[1]));
  ASSIGN_OR_RETURN(double z, ExecuteNodeT<double>(args[2]));
  return Any(std::make_shared<Point3>(x, y, z));
};

StatusOr<Any> SceneExecuter::CreateShapeNode(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 2, line, column));
  ASSIGN_OR_RETURN(auto s, ExecuteNodeObject<Shape>(args[0]));
  ASSIGN_OR_RETURN(auto m, ExecuteNodeT<std::shared_ptr<Material>>(args[1]));
  return Any(std::make_shared<ShapeNode>(s, m));
};

StatusOr<Any> SceneExecuter::CreateSphere(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 0, line, column));
  return Any(std::shared_ptr<SceneObject>(new SphereObject()));
};

StatusOr<Any> SceneExecuter::CreateTransformNode(
    const std::vector<const Node*>& args, int line, int column) {
  RETURN_IF_ERROR(ExpectSize(args, 0, line, column));
  return Any(std::make_shared<TransformNode>());
};

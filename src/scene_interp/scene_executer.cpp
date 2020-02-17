#include "scene_interp/scene_executer.h"

#include <algorithm>
#include <assert.h>
#include "algebra/matrix4.h"
#include "algebra/point3.h"
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"
#include "shape/cube.h"
#include "shape/sphere.h"
#include "shader/color.h"
#include "shader/light.h"
#include "shader/material.h"
#include "shape/cube.h"
#include "shape/sphere.h"

using namespace std::placeholders;

SceneExecuter::SceneExecuter(Parser* parser) : Executer(parser) {
  SetVariable("AXIS_X", std::make_any<double>(Matrix4::AXIS_X));
  SetVariable("AXIS_Y", std::make_any<double>(Matrix4::AXIS_Y));
  SetVariable("AXIS_Z", std::make_any<double>(Matrix4::AXIS_Z));

  SetVariable("root", std::any(std::shared_ptr<SceneObject>(new TransformNodeObject(this))));
  SetVariable("lights", std::any(std::shared_ptr<SceneObject>(new LightVector(this))));

  SetVariable("Color", std::any(SceneFunc(std::bind(&SceneExecuter::CreateColor, this, _1, _2))));
  SetVariable("Cube", std::any(SceneFunc(std::bind(&SceneExecuter::CreateCube, this, _1, _2))));
  SetVariable("Light", std::any(SceneFunc(std::bind(&SceneExecuter::CreateLight, this, _1, _2))));
  SetVariable("Material", std::any(SceneFunc(std::bind(&SceneExecuter::CreateMaterial, this, _1, _2))));
  SetVariable("Point3", std::any(SceneFunc(std::bind(&SceneExecuter::CreatePoint3, this, _1, _2))));
  SetVariable("ShapeNode", std::any(SceneFunc(std::bind(&SceneExecuter::CreateShapeNode, this, _1, _2))));
  SetVariable("Sphere", std::any(SceneFunc(std::bind(&SceneExecuter::CreateSphere, this, _1, _2))));
  SetVariable("TransformNode", std::any(SceneFunc(std::bind(&SceneExecuter::CreateTransformNode, this, _1, _2))));
}

std::any SceneExecuter::GetVariable(std::string_view name) const {
  auto it = variables_.find(name);
  return it == variables_.end() ? std::any() : it->second;
}

void SceneExecuter::SetVariable(std::string_view name, const std::any& any) {
  variables_[name] = any;
}

Status SceneExecuter::ExpectSize(
    const Token& token, const std::vector<const Node*>& args, int size) {
  return static_cast<int>(args.size()) == size ? Status() : Status(
      "Expecting " + std::to_string(size) + " argument(s)", token.line(), token.column());
}

StatusOr<std::any> SceneExecuter::ExecuteNode(const Node& node) {
  switch (node.token().type()) {
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

StatusOr<std::any> SceneExecuter::ExecuteAssignment(const Node& node) {
  std::string_view name = node.children()[0]->token().value();
  ASSIGN_OR_RETURN(auto value, ExecuteNode(*node.children()[1]));
  SetVariable(name, value);
  return value;
}

StatusOr<std::any> SceneExecuter::ExecuteDotAccessor(const Node& node) {
  ASSIGN_OR_RETURN(auto obj, ExecuteNodeT<std::shared_ptr<SceneObject>>(*node.children()[0]));
  return obj->Get(node.children()[1]->token());
}

StatusOr<std::any> SceneExecuter::ExecuteFunction(const Node& node) {
  const auto& children = node.children();
  ASSIGN_OR_RETURN(auto func, ExecuteNodeT<SceneFunc>(*children[0]));

  std::vector<const Node*> args;
  std::transform(children.begin() + 1, children.end(), std::back_inserter(args),
                 [](const std::unique_ptr<const Node>& n) { return n.get(); });

  return func(node.token(), args);
}

StatusOr<std::any> SceneExecuter::ExecuteIdentifier(const Node& node) {
  std::string_view name = node.token().value();

  if (!variables_.count(name)) {
    return Status(std::string(name) + " is undefined", node.token().line(),
                  node.token().column());
  }

  return GetVariable(name);
}

StatusOr<std::any> SceneExecuter::ExecuteNumber(const Node& node) {
  double value = strtod(node.token().value().data(), nullptr);
  return std::any(value);
}

StatusOr<std::any> SceneExecuter::CreateColor(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 3));
  ASSIGN_OR_RETURN(double r, ExecuteNodeT<double>(*args[0]));
  ASSIGN_OR_RETURN(double g, ExecuteNodeT<double>(*args[1]));
  ASSIGN_OR_RETURN(double b, ExecuteNodeT<double>(*args[2]));
  return std::any(std::make_shared<Color>(r, g, b));
};

StatusOr<std::any> SceneExecuter::CreateCube(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 0));
  return std::any(std::shared_ptr<Shape>(new Cube()));
};

StatusOr<std::any> SceneExecuter::CreateLight(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 2));
  ASSIGN_OR_RETURN(auto p, ExecuteNodeT<std::shared_ptr<Point3>>(*args[0]));
  ASSIGN_OR_RETURN(auto c, ExecuteNodeT<std::shared_ptr<Color>>(*args[1]));
  return std::any(std::make_shared<Light>(p, c));
};

StatusOr<std::any> SceneExecuter::CreateMaterial(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 3));
  ASSIGN_OR_RETURN(auto c, ExecuteNodeT<std::shared_ptr<Color>>(*args[0]));
  ASSIGN_OR_RETURN(double s, ExecuteNodeT<double>(*args[1]));
  ASSIGN_OR_RETURN(double r, ExecuteNodeT<double>(*args[2]));
  return std::any(std::make_shared<Material>(c, s, r));
};

StatusOr<std::any> SceneExecuter::CreatePoint3(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 3));
  ASSIGN_OR_RETURN(double x, ExecuteNodeT<double>(*args[0]));
  ASSIGN_OR_RETURN(double y, ExecuteNodeT<double>(*args[1]));
  ASSIGN_OR_RETURN(double z, ExecuteNodeT<double>(*args[2]));
  return std::any(std::make_shared<Point3>(x, y, z));
};

StatusOr<std::any> SceneExecuter::CreateShapeNode(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 2));
  ASSIGN_OR_RETURN(auto s, ExecuteNodeT<std::shared_ptr<Shape>>(*args[0]));
  ASSIGN_OR_RETURN(auto m, ExecuteNodeT<std::shared_ptr<Material>>(*args[1]));
  return std::any(std::shared_ptr<SceneObject>(new ShapeNodeObject(s, m)));
};

StatusOr<std::any> SceneExecuter::CreateSphere(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 0));
  return std::any(std::shared_ptr<Shape>(new Sphere()));
};

StatusOr<std::any> SceneExecuter::CreateTransformNode(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(ExpectSize(token, args, 0));
  return std::any(std::shared_ptr<SceneObject>(new TransformNodeObject(this)));
};

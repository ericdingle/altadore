#include "scene_interp/scene_executer.h"

#include <assert.h>
#include "algebra/point3.h"
#include "scene_interp/scene_lexer.h"
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "shader/color.h"
#include "shader/light.h"
#include "shader/material.h"
#include "shape/cube.h"
#include "shape/sphere.h"
#include "third_party/chaparral/src/executer/invokable.h"

SceneExecuter::SceneExecuter(Parser* parser) : Executer(parser) {
  SetVar("AXIS_X", std::make_shared<Variant>(static_cast<double>(
      Matrix4::AXIS_X)));
  SetVar("AXIS_Y", std::make_shared<Variant>(static_cast<double>(
      Matrix4::AXIS_Y)));
  SetVar("AXIS_Z", std::make_shared<Variant>(static_cast<double>(
      Matrix4::AXIS_Z)));
}

SceneExecuter::~SceneExecuter() {
}

void SceneExecuter::SetVar(const std::string& name,
                           const std::shared_ptr<const Variant>& var) {
  var_map_[name] = var;
}

bool SceneExecuter::ExecuteASTNode(const ASTNode* node,
                                   std::shared_ptr<const Variant>* var) {
  switch (node->token()->type()) {
    case SceneLexer::TYPE_DOT:
      return ExecuteDotAccessor(node, var);
    case SceneLexer::TYPE_EQUAL:
      return ExecuteAssignment(node, var);
    case SceneLexer::TYPE_IDENTIFIER:
      return ExecuteIdentifier(node, var);
    case SceneLexer::TYPE_NEW:
      return ExecuteNew(node, var);
    case SceneLexer::TYPE_NUMBER:
      return ExecuteNumber(node, var);
    default:
      assert(false);
      return false;
  }
}

bool SceneExecuter::ExecuteDotAccessor(const ASTNode* node,
                                       std::shared_ptr<const Variant>* var) {
  const ASTNode* obj_node = node->children()[0].get();
  std::shared_ptr<Invokable> object;
  if (!ExecuteASTNodeT(obj_node, &object))
    return false;

  const std::vector<std::unique_ptr<const ASTNode>>& children =
      node->children()[1]->children();
  const std::string& name = children[0]->token()->value();

  std::vector<std::shared_ptr<const Variant>> args;
  for (auto iter = std::next(children.begin()); iter != children.end(); ++iter) {
    std::shared_ptr<const Variant> arg;
    if (!ExecuteASTNode(iter->get(), &arg))
      return false;
    args.push_back(arg);
  }

  Invokable::Result result = object->Invoke(name, args, var);
  if (result != Invokable::RESULT_OK) {
    position_ = children[0]->token()->position();
    error_ = "ERROR";
    return false;
  }

  return true;
}

bool SceneExecuter::ExecuteAssignment(const ASTNode* node,
                                      std::shared_ptr<const Variant>* var) {
  const std::string& name = node->children()[0]->token()->value();

  std::shared_ptr<const Variant> right_var;
  if (!ExecuteASTNode(node->children()[1].get(), &right_var))
    return false;

  var_map_[name] = right_var;
  *var = right_var;
  return true;
}

bool SceneExecuter::ExecuteIdentifier(const ASTNode* node,
                                      std::shared_ptr<const Variant>* var) {
  const std::string& name = node->token()->value();

  if (var_map_.count(name) == 0) {
    position_ = node->token()->position();
    error_ = name + " is undefined";
    return false;
  }

  *var = var_map_[name];
  return true;
}

bool SceneExecuter::ExecuteNew(const ASTNode* node,
                               std::shared_ptr<const Variant>* var) {
  const std::vector<std::unique_ptr<const ASTNode>>& children =
      node->children()[0]->children();
  const std::string& name = children[0]->token()->value();

  std::vector<std::shared_ptr<const Variant>> args;
  for (auto iter = std::next(children.begin()); iter != children.end(); ++iter) {
    std::shared_ptr<const Variant> arg;
    if (!ExecuteASTNode(iter->get(), &arg))
      return false;
    args.push_back(arg);
  }

  std::shared_ptr<Invokable> object;
  Invokable::Result result = Invokable::RESULT_ERR_NAME;
  if (name == "Color")
    result = Color::Create(args, &object);
  else if (name == "Cube")
    result = Cube::Create(args, &object);
  else if (name == "Light")
    result = Light::Create(args, &object);
  else if (name == "Material")
    result = Material::Create(args, &object);
  else if (name == "Point3")
    result = Point3::Create(args, &object);
  else if (name == "ShapeNode")
    result = ShapeNode::Create(args, &object);
  else if (name == "Sphere")
    result = Sphere::Create(args, &object);
  else if (name == "TransformNode")
    result = TransformNode::Create(args, &object);

  if (result != Invokable::RESULT_OK) {
    position_ = children[0]->token()->position();
    error_ = "ERROR";
    return false;
  }

  var->reset(new Variant(object));
  return true;
}

bool SceneExecuter::ExecuteNumber(const ASTNode* node,
                                  std::shared_ptr<const Variant>* var) {
  double value = atof(node->token()->value().c_str());
  var->reset(new Variant(value));
  return true;
}

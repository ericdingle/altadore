#include "altadore/scene_interp/scene_executer.h"

#include "altadore/algebra/point3.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene/shape_node.h"
#include "altadore/scene/transform_node.h"
#include "altadore/shader/color.h"
#include "altadore/shader/light.h"
#include "altadore/shader/material.h"
#include "altadore/shape/cube.h"
#include "altadore/shape/sphere.h"
#include "altadore/util/invokable.h"
#include "bonavista/logging/assert.h"
#include "bonavista/string/format.h"

SceneExecuter::SceneExecuter(Parser* parser) : Executer(parser) {
  SetVar("AXIS_X", new Variant(static_cast<double>(Matrix4::AXIS_X)));
  SetVar("AXIS_Y", new Variant(static_cast<double>(Matrix4::AXIS_Y)));
  SetVar("AXIS_Z", new Variant(static_cast<double>(Matrix4::AXIS_Z)));
}

SceneExecuter::~SceneExecuter() {
}

void SceneExecuter::SetVar(const std::string& name, const Variant* var) {
  var_map_[name] = var;
}

bool SceneExecuter::ExecuteASTNode(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

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
      // This should never happen.
      ASSERT(false);
  }
}

bool SceneExecuter::ExecuteDotAccessor(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  const ASTNode* obj_node = node->children()[0];
  memory::scoped_refptr<Invokable> object;
  if (!ExecuteASTNodeT(obj_node, object.Receive()))
    return false;

  const std::vector<const ASTNode*>& children = node->children()[1]->children();
  const std::string& name = children[0]->token()->value();

  std::vector<memory::scoped_refptr<const Variant> > args;
  for (uint i = 1; i < children.size(); ++i) {
    memory::scoped_refptr<const Variant> arg;
    if (!ExecuteASTNode(children[i], arg.Receive()))
      return false;
    args.push_back(arg.ptr());
  }

  Invokable::Result result = object->Invoke(name, args, var);
  if (result != Invokable::RESULT_OK) {
    position_ = children[0]->token()->position();
    error_ = "ERROR";
    return false;
  }

  return true;
}

bool SceneExecuter::ExecuteAssignment(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  const std::string& name = node->children()[0]->token()->value();

  memory::scoped_refptr<const Variant> right_var;
  if (!ExecuteASTNode(node->children()[1], right_var.Receive()))
    return false;

  var_map_[name] = right_var;
  *var = right_var.Release();
  return true;
}

bool SceneExecuter::ExecuteIdentifier(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  const std::string& name = node->token()->value();

  if (var_map_.count(name) == 0) {
    position_ = node->token()->position();
    error_ = string::Format("%s is undefined", name.c_str());
    return false;
  }

  memory::scoped_refptr<const Variant> var_ref(var_map_[name]);
  *var = var_ref.Release();
  return true;
}

bool SceneExecuter::ExecuteNew(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  const std::vector<const ASTNode*>& children = node->children()[0]->children();
  const std::string& name = children[0]->token()->value();

  std::vector<memory::scoped_refptr<const Variant> > args;
  for (uint i = 1; i < children.size(); ++i) {
    memory::scoped_refptr<const Variant> arg;
    if (!ExecuteASTNode(children[i], arg.Receive()))
      return false;
    args.push_back(arg.ptr());
  }

  memory::scoped_refptr<Invokable> object;
  Invokable::Result result = Invokable::RESULT_ERR_NAME;
  if (name == "Color")
    result = Color::Create(args, object.Receive());
  else if (name == "Cube")
    result = Cube::Create(args, object.Receive());
  else if (name == "Light")
    result = Light::Create(args, object.Receive());
  else if (name == "Material")
    result = Material::Create(args, object.Receive());
  else if (name == "Point3")
    result = Point3::Create(args, object.Receive());
  else if (name == "ShapeNode")
    result = ShapeNode::Create(args, object.Receive());
  else if (name == "Sphere")
    result = Sphere::Create(args, object.Receive());
  else if (name == "TransformNode")
    result = TransformNode::Create(args, object.Receive());

  if (result != Invokable::RESULT_OK) {
    position_ = children[0]->token()->position();
    error_ = "ERROR";
    return false;
  }

  memory::scoped_refptr<const Variant> var_ref(new Variant(object.ptr()));
  *var = var_ref.Release();
  return true;
}

bool SceneExecuter::ExecuteNumber(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  double value = atof(node->token()->value().c_str());
  memory::scoped_refptr<const Variant> var_ref(new Variant(value));
  *var = var_ref.Release();
  return true;
}

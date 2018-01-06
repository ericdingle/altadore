#include "scene_interp/scene_object.h"

#include "scene_interp/scene_executer.h"

using namespace std::placeholders;

SceneObject::SceneObject(SceneExecuter* executer) : executer_(executer) {
}

StatusOr<Any> SceneObject::Get(const std::shared_ptr<SceneObject>& obj,
                               const Token& token) {
  return Status(token.value() + " is undefined", token.line(), token.column());
}

template <typename T>
StatusOr<T> SceneObject::ExecuteNodeT(const Node* node) {
  return executer_->ExecuteNodeT<T>(node);
}

ShapeNodeObject::ShapeNodeObject(const std::shared_ptr<const Shape>& shape,
                                 const std::shared_ptr<const Material>& material)
    : ShapeNode(shape, material), SceneObject(nullptr) {
}

StatusOr<Any> TransformNodeObject::Get(const std::shared_ptr<SceneObject>& obj,
                                       const Token& token) {
  auto ptr = std::dynamic_pointer_cast<TransformNodeObject>(obj);

  if (token.value() == "AddChild") {
    return Any(SceneFunc(std::bind(&TransformNodeObject::AddChild, ptr, _1, _2)));
  } else if (token.value() == "Rotate") {
    return Any(SceneFunc(std::bind(&TransformNodeObject::Rotate, ptr, _1, _2)));
  } else if (token.value() == "Scale") {
    return Any(SceneFunc(std::bind(&TransformNodeObject::Scale, ptr, _1, _2)));
  } else if (token.value() == "Translate") {
    return Any(SceneFunc(std::bind(&TransformNodeObject::Translate, ptr, _1, _2)));
  }

  return SceneObject::Get(obj, token);
}

StatusOr<Any> TransformNodeObject::AddChild(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(SceneExecuter::ExpectSize(token, args, 1));
  ASSIGN_OR_RETURN(auto obj, ExecuteNodeT<std::shared_ptr<SceneObject>>(args[0]));
  auto node = std::dynamic_pointer_cast<SceneNode>(obj);
  if (!node) {
    return Status("Expected type: SceneNode", args[0]->token().line(),
                  args[0]->token().column());
  }
  TransformNode::AddChild(node);
  return Any(obj);
}

StatusOr<Any> TransformNodeObject::Rotate(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(SceneExecuter::ExpectSize(token, args, 2));
  ASSIGN_OR_RETURN(double axis, ExecuteNodeT<double>(args[0]));
  if (axis < Matrix4::AXIS_X || axis > Matrix4::AXIS_Z) {
    return Status("Expected type: Matrix4::AXIS", args[0]->token().line(),
                  args[0]->token().column());
  }
  ASSIGN_OR_RETURN(double angle, ExecuteNodeT<double>(args[1]));
  TransformNode::Rotate(static_cast<Matrix4::Axis>(axis), angle);
  return Any();
}

StatusOr<Any> TransformNodeObject::Scale(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(SceneExecuter::ExpectSize(token, args, 3));
  ASSIGN_OR_RETURN(double x, ExecuteNodeT<double>(args[0]));
  ASSIGN_OR_RETURN(double y, ExecuteNodeT<double>(args[1]));
  ASSIGN_OR_RETURN(double z, ExecuteNodeT<double>(args[2]));
  TransformNode::Scale(x, y, z);
  return Any();
}

StatusOr<Any> TransformNodeObject::Translate(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(SceneExecuter::ExpectSize(token, args, 3));
  ASSIGN_OR_RETURN(double x, ExecuteNodeT<double>(args[0]));
  ASSIGN_OR_RETURN(double y, ExecuteNodeT<double>(args[1]));
  ASSIGN_OR_RETURN(double z, ExecuteNodeT<double>(args[2]));
  TransformNode::Translate(x, y, z);
  return Any();
}

LightVector::LightVector(SceneExecuter* executer) : SceneObject(executer) {
}

StatusOr<Any> LightVector::Get(const std::shared_ptr<SceneObject>& obj,
                               const Token& token) {
  auto ptr = std::dynamic_pointer_cast<LightVector>(obj);

  if (token.value() == "AddLight") {
    return Any(SceneFunc(std::bind(&LightVector::AddLight, ptr, _1, _2)));
  }

  return SceneObject::Get(obj, token);
}

StatusOr<Any> LightVector::AddLight(
    const Token& token, const std::vector<const Node*>& args) {
  RETURN_IF_ERROR(SceneExecuter::ExpectSize(token, args, 1));
  ASSIGN_OR_RETURN(auto light, ExecuteNodeT<std::shared_ptr<Light>>(args[0]));
  std::vector<std::shared_ptr<Light>>::push_back(light);
  return Any(light);
}

#include "altadore/scene/transform_node.h"

#include <limits>
#include "chaparral/executer/variant.h"

Invokable::Result TransformNode::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 0)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<TransformNode> node(new TransformNode());
  *object = node.Release();
  return RESULT_OK;
}

TransformNode::TransformNode() {
}

TransformNode::~TransformNode() {
}

Invokable::Result TransformNode::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  if (name == "AddChild")
    return InvokeAddChild(args);
  else if (name == "Rotate")
    return InvokeRotate(args);
  else if (name == "Scale")
    return InvokeScale(args);
  else if (name == "Translate")
    return InvokeTranslate(args);

  return RESULT_ERR_NAME;
}

Invokable::Result TransformNode::InvokeAddChild(
    const std::vector<memory::scoped_refptr<const Variant> >& args) {
  if (args.size() != 1)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<Invokable> object;
  if (!args[0]->Get(object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  SceneNode* node = dynamic_cast<SceneNode*>(object.ptr());
  if (!node)
    return RESULT_ERR_ARG_TYPE;

  AddChild(node);
  return RESULT_OK;
}

Invokable::Result TransformNode::InvokeRotate(
    const std::vector<memory::scoped_refptr<const Variant> >& args) {
  if (args.size() != 2)
    return RESULT_ERR_ARG_SIZE;

  double axis_d, angle;
  if (!args[0]->Get(&axis_d) || !args[1]->Get(&angle))
    return RESULT_ERR_ARG_TYPE;

  int axis_i = static_cast<int>(axis_d);
  if (!(axis_i >= Matrix4::AXIS_X && axis_i <= Matrix4::AXIS_Z))
    return RESULT_ERR_FAIL;

  Matrix4::Axis axis = static_cast<Matrix4::Axis>(axis_i);
  Rotate(axis, angle);
  return RESULT_OK;
}

Invokable::Result TransformNode::InvokeScale(
    const std::vector<memory::scoped_refptr<const Variant> >& args) {
  if (args.size() != 1 && args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  if (args.size() == 1) {
    double s;
    if (!args[0]->Get(&s))
      return RESULT_ERR_ARG_TYPE;
    Scale(s);
  } else {
    double x, y, z;
    if (!args[0]->Get(&x) || !args[1]->Get(&y) || !args[2]->Get(&z))
      return RESULT_ERR_ARG_TYPE;
    Scale(x, y, z);
  }

  return RESULT_OK;
}

Invokable::Result TransformNode::InvokeTranslate(
    const std::vector<memory::scoped_refptr<const Variant> >& args) {
  if (args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  double x, y, z;
  if (!args[0]->Get(&x) || !args[1]->Get(&y) || !args[2]->Get(&z))
    return RESULT_ERR_ARG_TYPE;

  Translate(x, y, z);
  return RESULT_OK;
}

void TransformNode::AddChild(SceneNode* child) {
  ASSERT(child);
  children_.push_back(child);
}

void TransformNode::Rotate(Matrix4::Axis axis, double angle) {
  transform_ = Matrix4::GetRotation(axis, angle) * transform_;
}

void TransformNode::Scale(double s) {
  transform_ = Matrix4::GetScaling(s) * transform_;
}

void TransformNode::Scale(double x, double y, double z) {
  transform_ = Matrix4::GetScaling(x, y, z) * transform_;
}

void TransformNode::Translate(double x, double y, double z) {
  transform_ = Matrix4::GetTranslation(x, y, z) * transform_;
}

void TransformNode::CalculateTransforms(const Matrix4& parent_transform) {
  Matrix4 total_transform = parent_transform * transform_;

  for (uint i = 0; i < children_.size(); ++i) {
    children_[i]->CalculateTransforms(total_transform);
  }
}

bool TransformNode::FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const {
  ASSERT(t);
  ASSERT(point);
  ASSERT(normal);
  ASSERT(material);

  *t = std::numeric_limits<double>::max();

  for (uint i = 0; i < children_.size(); ++i) {
    double t2;
    Point3 point2;
    Vector3 normal2;
    const Material* material2 = NULL;
    if (children_[i]->FindIntersection(ray, &t2, &point2, &normal2, &material2) && t2 < *t) {
      DASSERT(material2 != NULL);
      *t = t2;
      *point = point2;
      *normal = normal2;
      *material = material2;
    }
  }

  return *t != std::numeric_limits<double>::max();
}

bool TransformNode::HasIntersection(const Ray& ray) const {
  for (uint i = 0; i < children_.size(); ++i) {
    if (children_[i]->HasIntersection(ray)) {
      return true;
    }
  }

  return false;
}

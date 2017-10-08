#include "scene/shape_node.h"

#include "shader/material.h"
#include "shape/shape.h"
#include "third_party/chaparral/src/executer/variant.h"

Invokable::Result ShapeNode::Create(
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<Invokable>* object) {
  if (args.size() != 2)
    return RESULT_ERR_ARG_SIZE;

  std::shared_ptr<Invokable> shape_object;
  if (!args[0]->Get(&shape_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Shape> shape = std::dynamic_pointer_cast<Shape>(shape_object);
  if (!shape.get())
    return RESULT_ERR_ARG_TYPE;

  std::shared_ptr<Invokable> material_object;
  if (!args[1]->Get(&material_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Material> material = std::dynamic_pointer_cast<Material>(material_object);
  if (!material.get())
    return RESULT_ERR_ARG_TYPE;

  object->reset(new ShapeNode(shape, material));
  return RESULT_OK;
}

ShapeNode::ShapeNode(const std::shared_ptr<const Shape>& shape,
                     const std::shared_ptr<const Material>& material)
    : SceneNode(), shape_(shape), material_(material) {
}

ShapeNode::~ShapeNode() {
}

Invokable::Result ShapeNode::Invoke(
    const std::string& name,
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<const Variant>* var) {
  return RESULT_ERR_NAME;
}

void ShapeNode::CalculateTransforms(const Matrix4& parent_transform) {
  transform_ = parent_transform;
  transform_inverse_ = transform_.GetInverse();
  transform_inverse_transpose_ = transform_inverse_.GetTranspose();
}

bool ShapeNode::FindIntersection(const Ray& ray, double* t, Point3* point,
                                 Vector3* normal, const Material** material) const {
  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();

  if (!shape_->FindIntersection(Ray(origin, direction), t, point, normal))
    return false;

  *point = transform_ * *point;
  *normal = transform_inverse_transpose_ * *normal;
  normal->Normalize();
  *material = material_.get();
  return true;
}

bool ShapeNode::HasIntersection(const Ray& ray, double max_t) const {
  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();
  return shape_->HasIntersection(Ray(origin, direction), max_t);
}

#include "altadore/scene/shape_node.h"

#include "altadore/shader/material.h"
#include "altadore/shape/shape.h"

ShapeNode::ShapeNode(const std::shared_ptr<const Shape>& shape,
                     const std::shared_ptr<const Material>& material)
    : SceneNode(), shape_(shape), material_(material) {
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

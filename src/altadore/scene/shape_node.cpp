#include "altadore/scene/shape_node.h"

#include "altadore/shape/shape.h"
#include "altadore/visual/material.h"

ShapeNode::ShapeNode(const Shape* shape, const Material* material) : SceneNode(), shape_(shape), material_(material) {
  DASSERT(shape != NULL);
  DASSERT(material != NULL);
}

ShapeNode::~ShapeNode() {
}

void ShapeNode::CalculateTransforms(const Matrix4& parent_transform) {
  transform_ = parent_transform;
  transform_inverse_ = transform_.GetInverse();
  transform_inverse_transpose_ = transform_inverse_.GetTranspose();
}

bool ShapeNode::FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const {
  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();
  Ray ray2(origin, direction);

  if (!shape_->FindIntersection(ray2, t, point, normal))
    return false;

  *point = transform_ * *point;
  *normal = transform_inverse_transpose_ * *normal;
  normal->Normalize();
  *material = material_.ptr();
  return true;
}

bool ShapeNode::HasIntersection(const Ray& ray) const {
  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();
  Ray ray2(origin, direction);

  return shape_->HasIntersection(ray2);
}

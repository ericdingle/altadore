#include "altadore/scene/transform_node.h"

#include <assert.h>
#include <limits>

void TransformNode::AddChild(const std::shared_ptr<SceneNode>& child) {
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

  for (const auto& child : children_) {
    child->CalculateTransforms(total_transform);
  }
}

bool TransformNode::FindIntersection(const Ray& ray, double* t, Point3* point,
                                     Vector3* normal, const Material** material) const {
  *t = std::numeric_limits<double>::max();

  for (const auto& child : children_) {
    double t2 = 0.0;
    Point3 point2;
    Vector3 normal2;
    const Material* material2 = NULL;
    if (child->FindIntersection(ray, &t2, &point2, &normal2, &material2) && t2 < *t) {
      assert(material2);
      *t = t2;
      *point = point2;
      *normal = normal2;
      *material = material2;
    }
  }

  return *t != std::numeric_limits<double>::max();
}

bool TransformNode::HasIntersection(const Ray& ray, double max_t) const {
  for (const auto& child : children_) {
    if (child->HasIntersection(ray, max_t)) {
      return true;
    }
  }

  return false;
}

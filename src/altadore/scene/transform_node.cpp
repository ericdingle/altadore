#include "altadore/scene/transform_node.h"

#include <limits>

TransformNode::TransformNode() : SceneNode() {
}

TransformNode::~TransformNode() {
}

void TransformNode::AddChild(SceneNode* child) {
  ASSERT(child);
  children_.push_back(child);
}

void TransformNode::Rotate(char axis, double angle) {
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

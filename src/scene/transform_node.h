#ifndef SCENE_TRANSFORM_NODE_H_
#define SCENE_TRANSFORM_NODE_H_

#include <memory>
#include <vector>
#include "algebra/matrix4.h"
#include "scene/scene_node.h"

class TransformNode : public SceneNode {
 public:
  TransformNode() = default;
  TransformNode(const TransformNode&) = delete;
  TransformNode& operator=(const TransformNode&) = delete;
  ~TransformNode() override = default;

  void AddChild(const std::shared_ptr<SceneNode>& node);

  void Rotate(Matrix4::Axis axis, double angle);
  void Scale(double s);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);

  void CalculateTransforms(const Matrix4& parent_transform) override;
  bool FindIntersection(const Ray& ray, double* t, Point3* point,
                        Vector3* normal, const Material** material) const override;
  bool HasIntersection(const Ray& ray, double max_t) const override;

 protected:
  const std::vector<std::shared_ptr<SceneNode>>& children() { return children_; }
  const Matrix4& transform() { return transform_; }

 private:
  std::vector<std::shared_ptr<SceneNode>> children_;
  Matrix4 transform_;
};

#endif  // SCENE_TRANSFORM_NODE_H_

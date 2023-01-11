#ifndef SCENE_SHAPE_NODE_H_
#define SCENE_SHAPE_NODE_H_

#include <memory>

#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"

class Shape;

class ShapeNode : public SceneNode {
 public:
  ShapeNode(const std::shared_ptr<const Shape>& shape,
            const std::shared_ptr<const Material>& material);
  ShapeNode(const ShapeNode&) = delete;
  ShapeNode& operator=(const ShapeNode&) = delete;
  ~ShapeNode() override = default;

  void CalculateTransforms(const Matrix4& parent_transform) override;
  bool FindIntersection(const Ray& ray, double* t, Point3* point,
                        Vector3* normal, const Material** material) const override;
  bool HasIntersection(const Ray& ray, double max_t) const override;

 protected:
  const Matrix4& transform() const { return transform_; }
  const Matrix4& transform_inverse() const { return transform_inverse_; }
  const Matrix4& transform_inverse_transpose() const { return transform_inverse_transpose_; }

 private:
  const std::shared_ptr<const Shape> shape_;
  const std::shared_ptr<const Material> material_;

  Matrix4 transform_;
  Matrix4 transform_inverse_;
  Matrix4 transform_inverse_transpose_;
};

#endif  // SCENE_SHAPE_NODE_H_

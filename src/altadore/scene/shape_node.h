#ifndef ALTADORE_SCENE_SHAPE_NODE_H_
#define ALTADORE_SCENE_SHAPE_NODE_H_

#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class Shape;

class ShapeNode : public SceneNode {
 public:
  ShapeNode(const Shape* shape, const Material* material);
  ~ShapeNode();

  void CalculateTransforms(const Matrix4& parent_transform);
  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const;
  bool HasIntersection(const Ray& ray) const;

 protected:
  const Matrix4& transform() const { return transform_; }
  const Matrix4& transform_inverse() const { return transform_inverse_; }
  const Matrix4& transform_inverse_transpose() const { return transform_inverse_transpose_; }

 private:
  Matrix4 transform_;
  Matrix4 transform_inverse_;
  Matrix4 transform_inverse_transpose_;

  const memory::scoped_refptr<const Shape> shape_;
  const memory::scoped_refptr<const Material> material_;

  DISALLOW_COPY_AND_ASSIGN(ShapeNode);
};

#endif

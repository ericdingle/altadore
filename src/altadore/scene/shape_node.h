#ifndef ALTADORE_SCENE_SHAPE_NODE_H_
#define ALTADORE_SCENE_SHAPE_NODE_H_

#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"
#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_refptr.h"

class Shape;

class ShapeNode : public SceneNode {
 public:
  static Result Create(
      const std::vector<scoped_refptr<const Variant> >& args,
      Invokable** object);

  ShapeNode(const Shape* shape, const Material* material);
  virtual ~ShapeNode();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  virtual void CalculateTransforms(const Matrix4& parent_transform);
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const;
  virtual bool HasIntersection(const Ray& ray) const;

 protected:
  const Matrix4& transform() const { return transform_; }
  const Matrix4& transform_inverse() const { return transform_inverse_; }
  const Matrix4& transform_inverse_transpose() const { return transform_inverse_transpose_; }

 private:
  const scoped_refptr<const Shape> shape_;
  const scoped_refptr<const Material> material_;

  Matrix4 transform_;
  Matrix4 transform_inverse_;
  Matrix4 transform_inverse_transpose_;

  DISALLOW_COPY_AND_ASSIGN(ShapeNode);
};

#endif

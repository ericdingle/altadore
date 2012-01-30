#ifndef ALTADORE_SCENE_TRANSFORM_NODE_H_
#define ALTADORE_SCENE_TRANSFORM_NODE_H_

#include <vector>
#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class TransformNode : public SceneNode {
 public:
  TransformNode();
  virtual ~TransformNode();

  void AddChild(SceneNode* node);

  void Rotate(char axis, double angle);
  void Scale(double s);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);

  virtual void CalculateTransforms(const Matrix4& parent_transform);
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const;
  virtual bool HasIntersection(const Ray& ray) const;

 protected:
  const std::vector<memory::scoped_refptr<SceneNode> >& children() { return children_; }
  const Matrix4& transform() { return transform_; }

 private:
  std::vector<memory::scoped_refptr<SceneNode> > children_;
  Matrix4 transform_;

  DISALLOW_COPY_AND_ASSIGN(TransformNode);
};

#endif

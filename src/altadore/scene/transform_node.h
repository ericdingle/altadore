#ifndef ALTADORE_SCENE_TRANSFORM_NODE_H_
#define ALTADORE_SCENE_TRANSFORM_NODE_H_

#include <vector>
#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"
#include "bonavista/memory/scoped_ref.h"

class TransformNode : public SceneNode {
public:
  TransformNode();
  ~TransformNode();

  void AddChild(SceneNode* node);

  void Rotate(char axis, double angle);
  void Scale(double s);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);

  void CalculateTransforms(const Matrix4& parent_transform);
  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const;
  bool HasIntersection(const Ray& ray) const;

protected:
  const std::vector<memory::scoped_ref<SceneNode> >& children() { return children_; }
  const Matrix4& transform() { return transform_; }

private:
  TransformNode(const TransformNode&);
  void operator=(const TransformNode&);

  std::vector<memory::scoped_ref<SceneNode> > children_;
  Matrix4 transform_;
};

#endif

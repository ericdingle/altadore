#ifndef ALTADORE_SCENE_SCENE_NODE_H_
#define ALTADORE_SCENE_SCENE_NODE_H_

#include "altadore/algebra/ray.h"
#include "bonavista/memory/ref_count.h"

class Material;
class Matrix4;

class SceneNode : public memory::RefCount {
public:
  SceneNode();
  virtual ~SceneNode();

  virtual void CalculateTransforms(const Matrix4& parent_transform) =0;
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const =0;
  virtual bool HasIntersection(const Ray& ray) const =0;

private:
  SceneNode(const SceneNode&);
  void operator=(const SceneNode&);
};

#endif

#ifndef ALTADORE_SCENE_SCENE_NODE_H_
#define ALTADORE_SCENE_SCENE_NODE_H_

#include "altadore/algebra/ray.h"
#include "altadore/util/invokable.h"
#include "bonavista/util/macros.h"

class Material;
class Matrix4;

class SceneNode : public Invokable {
 public:
  SceneNode();
  virtual ~SceneNode();

  virtual void CalculateTransforms(const Matrix4& parent_transform) =0;
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const =0;
  virtual bool HasIntersection(const Ray& ray) const =0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SceneNode);
};

#endif

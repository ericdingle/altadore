#ifndef SCENE_SCENE_NODE_H_
#define SCENE_SCENE_NODE_H_

#include "algebra/ray.h"
#include "third_party/chaparral/src/executer/invokable.h"

class Material;
class Matrix4;

class SceneNode : public Invokable {
 public:
  SceneNode();
  SceneNode(const SceneNode&) = delete;
  SceneNode& operator=(const SceneNode&) = delete;
  virtual ~SceneNode();

  virtual void CalculateTransforms(const Matrix4& parent_transform) = 0;
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point,
                                Vector3* normal, const Material** material) const = 0;
  virtual bool HasIntersection(const Ray& ray, double max_t) const = 0;
};

#endif  // SCENE_SCENE_NODE_H_

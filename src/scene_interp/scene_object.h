#ifndef EXECUTER_SCENE_OBJECT_H_
#define EXECUTER_SCENE_OBJECT_H_

#include <string>
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "shape/cube.h"
#include "shape/sphere.h"
#include "third_party/bonavista/src/util/status_or.h"
#include "third_party/chaparral/src/executer/any.h"

class SceneObject {
 public:
  SceneObject() = default;
  SceneObject(const SceneObject&) = delete;
  SceneObject& operator=(const SceneObject&) = delete;
  virtual ~SceneObject() = default;

  virtual StatusOr<Any> Get(const std::string& name);
};

class CubeObject : public Cube, public SceneObject {
 public:
  CubeObject() = default;
  CubeObject(const CubeObject&) = delete;
  CubeObject& operator=(const CubeObject&) = delete;
  ~CubeObject() override = default;
};

class ShapeNodeObject : public ShapeNode, public SceneObject {
 public:
  using ShapeNode::ShapeNode;
  ShapeNodeObject(const ShapeNodeObject&) = delete;
  ShapeNodeObject& operator=(const ShapeNodeObject&) = delete;
  ~ShapeNodeObject() override = default;
};

class SphereObject : public Sphere, public SceneObject {
 public:
  SphereObject() = default;
  SphereObject(const SphereObject&) = delete;
  SphereObject& operator=(const SphereObject&) = delete;
  ~SphereObject() override = default;
};

class TransformNodeObject : public TransformNode, public SceneObject {
 public:
  TransformNodeObject() = default;
  TransformNodeObject(const TransformNodeObject&) = delete;
  TransformNodeObject& operator=(const TransformNodeObject&) = delete;
  ~TransformNodeObject() override = default;
};

#endif  // EXECUTER_SCENE_OBJECT_H_

#ifndef SCENE_INTERP_SCENE_OBJECT_H_
#define SCENE_INTERP_SCENE_OBJECT_H_

#include <memory>
#include <string>
#include <vector>
#include "scene/shape_node.h"
#include "scene/transform_node.h"
#include "shader/light.h"
#include "third_party/bonavista/src/lexer/token.h"
#include "third_party/bonavista/src/parser/node.h"
#include "third_party/bonavista/src/util/status_or.h"
#include "third_party/chaparral/src/executer/any.h"

class SceneExecuter;

class SceneObject {
 public:
  explicit SceneObject(SceneExecuter* executer);
  SceneObject(const SceneObject&) = delete;
  SceneObject& operator=(const SceneObject&) = delete;
  virtual ~SceneObject() = default;

  virtual StatusOr<Any> Get(const std::shared_ptr<SceneObject>& obj,
                            const Token& token);

 protected:
  template <typename T>
  StatusOr<T> ExecuteNodeT(const Node* node);

 private:
  SceneExecuter* executer_;
};

class ShapeNodeObject : public ShapeNode, public SceneObject {
 public:
  ShapeNodeObject(const std::shared_ptr<const Shape>& shape,
                  const std::shared_ptr<const Material>& material);
  ShapeNodeObject(const ShapeNodeObject&) = delete;
  ShapeNodeObject& operator=(const ShapeNodeObject&) = delete;
  ~ShapeNodeObject() override = default;
};

class TransformNodeObject : public TransformNode, public SceneObject {
 public:
  using SceneObject::SceneObject;
  TransformNodeObject(const TransformNodeObject&) = delete;
  TransformNodeObject& operator=(const TransformNodeObject&) = delete;
  ~TransformNodeObject() override = default;

  StatusOr<Any> Get(const std::shared_ptr<SceneObject>& obj, const Token& token)
      override;

 private:
  StatusOr<Any> AddChild(const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> Rotate(const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> Scale(const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> Translate(const Token& token, const std::vector<const Node*>& args);
};

class LightVector : public std::vector<std::shared_ptr<Light>>, public SceneObject {
 public:
  explicit LightVector(SceneExecuter* executer);
  LightVector(const LightVector&) = delete;
  LightVector& operator=(const LightVector&) = delete;
  ~LightVector() override = default;

  StatusOr<Any> Get(const std::shared_ptr<SceneObject>& obj, const Token& token)
      override;

 private:
  StatusOr<Any> AddLight(const Token& token, const std::vector<const Node*>& args);
};

#endif  // SCENE_INTERP_SCENE_OBJECT_H_

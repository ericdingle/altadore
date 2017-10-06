#ifndef SCENE_TRANSFORM_NODE_H_
#define SCENE_TRANSFORM_NODE_H_

#include <memory>
#include "algebra/matrix4.h"
#include "scene/scene_node.h"

class TransformNode : public SceneNode {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  TransformNode();
  TransformNode(const TransformNode&) = delete;
  TransformNode& operator=(const TransformNode&) = delete;
  virtual ~TransformNode();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  void AddChild(const std::shared_ptr<SceneNode>& node);

  void Rotate(Matrix4::Axis axis, double angle);
  void Scale(double s);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);

  virtual void CalculateTransforms(const Matrix4& parent_transform);
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point,
                                Vector3* normal, const Material** material) const;
  virtual bool HasIntersection(const Ray& ray, double max_t) const;

 protected:
  const std::vector<std::shared_ptr<SceneNode>>& children() { return children_; }
  const Matrix4& transform() { return transform_; }

 private:
  Result InvokeAddChild(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);
  Result InvokeRotate(
      const std::vector<std::shared_ptr<const Variant>>& args);
  Result InvokeScale(
      const std::vector<std::shared_ptr<const Variant>>& args);
  Result InvokeTranslate(
      const std::vector<std::shared_ptr<const Variant>>& args);

  std::vector<std::shared_ptr<SceneNode>> children_;
  Matrix4 transform_;
};

#endif  // SCENE_TRANSFORM_NODE_H_

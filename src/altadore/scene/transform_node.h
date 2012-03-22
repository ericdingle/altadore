#ifndef ALTADORE_SCENE_TRANSFORM_NODE_H_
#define ALTADORE_SCENE_TRANSFORM_NODE_H_

#include <vector>
#include "altadore/algebra/matrix4.h"
#include "altadore/scene/scene_node.h"
#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_refptr.h"

class TransformNode : public SceneNode {
 public:
  static Result Create(
      const std::vector<scoped_refptr<const Variant> >& args,
      Invokable** object);

  TransformNode();
  virtual ~TransformNode();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  void AddChild(SceneNode* node);

  void Rotate(Matrix4::Axis axis, double angle);
  void Scale(double s);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);

  virtual void CalculateTransforms(const Matrix4& parent_transform);
  virtual bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const;
  virtual bool HasIntersection(const Ray& ray) const;

 protected:
  const std::vector<scoped_refptr<SceneNode> >& children() { return children_; }
  const Matrix4& transform() { return transform_; }

 private:
  Result InvokeAddChild(
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);
  Result InvokeRotate(
      const std::vector<scoped_refptr<const Variant> >& args);
  Result InvokeScale(
      const std::vector<scoped_refptr<const Variant> >& args);
  Result InvokeTranslate(
      const std::vector<scoped_refptr<const Variant> >& args);

  std::vector<scoped_refptr<SceneNode> > children_;
  Matrix4 transform_;

  DISALLOW_COPY_AND_ASSIGN(TransformNode);
};

#endif

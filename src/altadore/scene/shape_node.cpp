#include "altadore/scene/shape_node.h"

#include "altadore/shader/material.h"
#include "altadore/shape/shape.h"
#include "bonavista/logging/assert.h"
#include "chaparral/executer/variant.h"

Invokable::Result ShapeNode::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 2)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<Invokable> shape_object;
  if (!args[0]->Get(shape_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Shape* shape = dynamic_cast<Shape*>(shape_object.ptr());
  if (!shape)
    return RESULT_ERR_ARG_TYPE;

  memory::scoped_refptr<Invokable> material_object;
  if (!args[1]->Get(material_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Material* material = dynamic_cast<Material*>(material_object.ptr());
  if (!material)
    return RESULT_ERR_ARG_TYPE;

  memory::scoped_refptr<ShapeNode> node(new ShapeNode(shape, material));
  *object = node.Release();
  return RESULT_OK;
}

ShapeNode::ShapeNode(const Shape* shape, const Material* material)
    : SceneNode(), shape_(shape), material_(material) {
  ASSERT(shape);
  ASSERT(material);
}

ShapeNode::~ShapeNode() {
}

Invokable::Result ShapeNode::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

void ShapeNode::CalculateTransforms(const Matrix4& parent_transform) {
  transform_ = parent_transform;
  transform_inverse_ = transform_.GetInverse();
  transform_inverse_transpose_ = transform_inverse_.GetTranspose();
}

bool ShapeNode::FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal, const Material** material) const {
  ASSERT(t);
  ASSERT(point);
  ASSERT(normal);
  ASSERT(material);

  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();

  if (!shape_->FindIntersection(Ray(origin, direction), t, point, normal))
    return false;

  *point = transform_ * *point;
  *normal = transform_inverse_transpose_ * *normal;
  normal->Normalize();
  *material = material_.ptr();
  return true;
}

bool ShapeNode::HasIntersection(const Ray& ray) const {
  Point3 origin = transform_inverse_ * ray.origin();
  Vector3 direction = transform_inverse_ * ray.direction();
  return shape_->HasIntersection(Ray(origin, direction));
}

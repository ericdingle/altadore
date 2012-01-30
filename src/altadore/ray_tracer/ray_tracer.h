#ifndef ALTADORE_RAY_TRACER_RAY_TRACER_H_
#define ALTADORE_RAY_TRACER_RAY_TRACER_H_

#include "altadore/scene/transform_node.h"
#include "altadore/visual/light_vector.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class RayTracer {
 public:
  RayTracer(const TransformNode* root, const LightVector* lights);
  ~RayTracer();

  bool Render(const char* fileName, int width, int height, bool anti_alias);

 protected:
  Color GetColor(const Ray& ray);
  Color GetAbsorbedColor(const Point3& point, const Vector3& normal, const Material* material);
  Color GetReflectedColor(const Ray& ray, const Point3& point, const Vector3& normal, const Material* material);

 private:
  memory::scoped_refptr<const TransformNode> root_;
  memory::scoped_refptr<const LightVector> lights_;

  DISALLOW_COPY_AND_ASSIGN(RayTracer);
};

#endif

#ifndef ALTADORE_RAY_TRACER_RAY_TRACER_H_
#define ALTADORE_RAY_TRACER_RAY_TRACER_H_

#include "altadore/scene/transform_node.h"
#include "altadore/visual/light_vector.h"

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
  RayTracer(const RayTracer&);
  void operator=(const RayTracer&);

  memory::scoped_ref<const TransformNode> root_;
  memory::scoped_ref<const LightVector> lights_;
};

#endif

#ifndef ALTADORE_RAY_TRACER_RAY_TRACER_H_
#define ALTADORE_RAY_TRACER_RAY_TRACER_H_

#include <memory>
#include "altadore/scene/transform_node.h"
#include "altadore/shader/light_vector.h"
#include "bonavista/base/macros.h"

class RayTracer {
 public:
  RayTracer(const std::shared_ptr<const TransformNode>& root,
            const std::shared_ptr<const LightVector>& lights);
  ~RayTracer();

  bool Render(const char* file_name, int width, int height, bool anti_alias);

 protected:
  Color GetColor(const Ray& ray);
  Color GetAbsorbedColor(const Point3& point, const Vector3& normal, const Material* material);
  Color GetReflectedColor(const Ray& ray, const Point3& point, const Vector3& normal, const Material* material);

 private:
  std::shared_ptr<const TransformNode> root_;
  std::shared_ptr<const LightVector> lights_;

  DISALLOW_COPY_AND_ASSIGN(RayTracer);
};

#endif

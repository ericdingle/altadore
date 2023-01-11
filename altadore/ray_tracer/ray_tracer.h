#ifndef RAY_TRACER_RAY_TRACER_H_
#define RAY_TRACER_RAY_TRACER_H_

#include <memory>
#include <vector>

#include "altadore/scene/transform_node.h"
#include "altadore/shader/light.h"

class RayTracer {
 public:
  RayTracer(const TransformNode& root,
            const std::vector<std::shared_ptr<Light>>& lights);
  RayTracer(const RayTracer&) = delete;
  RayTracer operator=(const RayTracer&) = delete;
  ~RayTracer() = default;

  bool Render(const char* file_name, int width, int height, bool anti_alias);

 protected:
  Color GetColor(const Ray& ray);
  Color GetAbsorbedColor(const Point3& point, const Vector3& normal, const Material* material);
  Color GetReflectedColor(const Ray& ray, const Point3& point, const Vector3& normal, const Material* material);

 private:
  const TransformNode& root_;
  const std::vector<std::shared_ptr<Light>>& lights_;
};

#endif  // RAY_TRACER_RAY_TRACER_H_

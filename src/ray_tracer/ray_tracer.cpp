#include "ray_tracer/ray_tracer.h"

#include <assert.h>
#include <cstdint>
#include <math.h>
#include "algebra/algebra_constants.h"
#include "algebra/matrix4.h"
#include "image/bitmap.h"
#include "ray_tracer/ray_tracer_constants.h"
#include "shader/material.h"

RayTracer::RayTracer(const std::shared_ptr<const TransformNode>& root,
                     const std::shared_ptr<const LightVector>& lights)
    : root_(root), lights_(lights) {
}

RayTracer::~RayTracer() {
}

bool RayTracer::Render(const char* file_name, int width, int height, bool anti_alias) {
  if (anti_alias) {
    width *= 2;
    height *= 2;
  }

  Bitmap bitmap(width, height);

  // The modeling matrix transforms from bitmap coordinates to plane coordinates.
  // First invert the y-axis. Then translate the origin to half the width and height of the image.
  Matrix4 m = Matrix4::GetTranslation(-width/2, height/2, 0) * Matrix4::GetScaling(1, -1, 1);

  // The viewing angle is 45 degrees. The adjacent is 1 unit (the distance from the origin to the plane).
  // Scale the scene according to the opposite.
  double h = 2 * tan(45/2 * kPi/180);
  Matrix4 s = Matrix4::GetScaling(h/height, h/height, 1);

  // Calculate the bitmap point to world matrix.
  Matrix4 p2w = s * m;

  Point3 origin(0, 0, 1);
  Ray ray(origin, Vector3());

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Point3 plane = p2w * Point3(x, y, 0);
      Vector3 direction = plane - origin;
      direction.Normalize();
      ray.set_direction(direction);

      Color color = GetColor(ray);
      Bitmap::Color c;
      c.r = static_cast<uint8_t>(255 * color.r());
      c.g = static_cast<uint8_t>(255 * color.g());
      c.b = static_cast<uint8_t>(255 * color.b());
      bitmap.Set(x, y, c);
    }
  }

  if (anti_alias) {
    bitmap.AntiAlias();
  }

  return bitmap.Save(file_name);
}

Color RayTracer::GetColor(const Ray& ray) {
  double t;
  Point3 point;
  Vector3 normal;
  const Material* material = NULL;
  if (!root_->FindIntersection(ray, &t, &point, &normal, &material))
    return Color();
  assert(material);

  double reflectivity = material->reflectivity();
  double absorptivity = 1 - reflectivity;

  Color color;
  if (absorptivity != 0) {
    color += GetAbsorbedColor(point, normal, material) * absorptivity;
  }
  if (reflectivity != 0) {
    color += GetReflectedColor(ray, point, normal, material) * reflectivity;
  }

  return color;
}

Color RayTracer::GetAbsorbedColor(const Point3& point, const Vector3& normal, const Material* material) {
  // color = ambient_intensity
  Color color;
  color += kAmbientIntensity;

  // color += diffuse_intensity[i] * light_color[i]
  for (const auto& light : lights_->lights()) {
    Vector3 light_dir = *light->position() - point;
    double max_t = light_dir.Length();
    light_dir.Normalize();
    Ray light_ray(point, light_dir);

    if (root_->HasIntersection(light_ray, max_t))
      continue;

    double diffuse_intensity = light_dir.Dot(normal);
    if (diffuse_intensity <= 0)
      continue;

    color += *light->color() * diffuse_intensity;
  }

  // color *= material_color
  color *= *material->color();

  return color;
}

Color RayTracer::GetReflectedColor(const Ray& ray, const Point3& point, const Vector3& normal, const Material* material) {
  // color = reflected_color
  Vector3 reflected_dir = ray.direction() - normal * 2 * normal.Dot(ray.direction());
  reflected_dir.Normalize();
  Ray reflected_ray(point, reflected_dir);
  Color color = GetColor(reflected_ray);

  // color += specular_intensity[i] * light_color[i]
  for (const auto& light : lights_->lights()) {
    Vector3 light_dir = *light->position() - point;
    double max_t = light_dir.Length();
    light_dir.Normalize();
    Ray light_ray(point, light_dir);

    if (root_->HasIntersection(light_ray, max_t))
      continue;

    Vector3 reflection = -light_dir + normal * 2 * light_dir.Dot(normal);
    reflection.Normalize();

    double specular_intensity = reflection.Dot(-ray.direction());
    if (specular_intensity <= 0)
      continue;

    specular_intensity = pow(specular_intensity, material->shininess());
    color += *light->color() * specular_intensity;
  }

  return color;
}

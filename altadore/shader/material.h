#ifndef SHADER_MATERIAL_H_
#define SHADER_MATERIAL_H_

#include "altadore/shader/color.h"

class Material {
 public:
  Material(const std::shared_ptr<const Color>& color, double shininess,
           double reflectivity);
  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;
  ~Material() = default;

  const Color* color() const { return color_.get(); }
  double shininess() const { return shininess_; }
  double reflectivity() const { return reflectivity_; }

 private:
  const std::shared_ptr<const Color> color_;
  const double shininess_;
  const double reflectivity_;
};

#endif  // SHADER_MATERIAL_H_

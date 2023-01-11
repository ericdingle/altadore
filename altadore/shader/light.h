#ifndef SHADER_LIGHT_H_
#define SHADER_LIGHT_H_

#include "altadore/algebra/point3.h"
#include "altadore/shader/color.h"

class Light {
 public:
  Light(const std::shared_ptr<const Point3>& position,
        const std::shared_ptr<const Color>& color);
  Light(const Light&) = delete;
  Light& operator=(const Light&) = delete;
  ~Light() = default;

  const Point3* position() const { return position_.get(); }
  const Color* color() const { return color_.get(); }

 private:
  const std::shared_ptr<const Point3> position_;
  const std::shared_ptr<const Color> color_;
};

#endif  // SHADER_LIGHT_H_

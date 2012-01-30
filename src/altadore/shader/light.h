#ifndef ALTADORE_SHADER_LIGHT_H_
#define ALTADORE_SHADER_LIGHT_H_

#include "altadore/algebra/point3.h"
#include "altadore/shader/color.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class Light : public memory::RefCount {
 public:
  Light(const Point3* position, const Color* color);
  ~Light();

  const Point3* position() const { return position_.ptr(); }
  const Color* color() const { return color_.ptr(); }

 private:
  const memory::scoped_refptr<const Point3> position_;
  const memory::scoped_refptr<const Color> color_;

  DISALLOW_COPY_AND_ASSIGN(Light);
};

#endif

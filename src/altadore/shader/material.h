#ifndef ALTADORE_SHADER_MATERIAL_H_
#define ALTADORE_SHADER_MATERIAL_H_

#include "altadore/shader/color.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class Material : public memory::RefCount {
 public:
  Material(const Color* color, double shininess, double reflectivity);
  ~Material();

  const Color* color() const { return color_.ptr(); }
  double shininess() const { return shininess_; }
  double reflectivity() const { return reflectivity_; }

 private:
  const memory::scoped_refptr<const Color> color_;
  const double shininess_;
  const double reflectivity_;

  DISALLOW_COPY_AND_ASSIGN(Material);
};

#endif

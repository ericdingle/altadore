#ifndef ALTADORE_SHADER_MATERIAL_H_
#define ALTADORE_SHADER_MATERIAL_H_

#include "altadore/shader/color.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"

class Material : public memory::RefCount {
public:
  Material(const Color* color, double shininess, double reflectivity);
  ~Material();

  const Color* color() const { return color_.ptr(); }
  double shininess() const { return shininess_; }
  double reflectivity() const { return reflectivity_; }

private:
  Material(const Material&);
  void operator=(const Material&);

  memory::scoped_refptr<const Color> color_;
  double shininess_;
  double reflectivity_;
};

#endif

#ifndef ALTADORE_SHADER_MATERIAL_H_
#define ALTADORE_SHADER_MATERIAL_H_

#include "altadore/shader/color.h"
#include "altadore/util/invokable.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class Material : public Invokable {
 public:
  static Result Create(
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      Invokable** object);

  Material(const Color* color, double shininess, double reflectivity);
  ~Material();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      const Variant** var);

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

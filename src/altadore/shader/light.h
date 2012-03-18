#ifndef ALTADORE_SHADER_LIGHT_H_
#define ALTADORE_SHADER_LIGHT_H_

#include "altadore/algebra/point3.h"
#include "altadore/shader/color.h"
#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_refptr.h"
#include "chaparral/executer/invokable.h"

class Light : public Invokable {
 public:
  static Result Create(
      const std::vector<scoped_refptr<const Variant> >& args,
      Invokable** object);

  Light(const Point3* position, const Color* color);
  virtual ~Light();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  const Point3* position() const { return position_.ptr(); }
  const Color* color() const { return color_.ptr(); }

 private:
  const scoped_refptr<const Point3> position_;
  const scoped_refptr<const Color> color_;

  DISALLOW_COPY_AND_ASSIGN(Light);
};

#endif

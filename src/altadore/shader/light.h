#ifndef ALTADORE_SHADER_LIGHT_H_
#define ALTADORE_SHADER_LIGHT_H_

#include <memory>
#include "altadore/algebra/point3.h"
#include "altadore/shader/color.h"
#include "bonavista/base/macros.h"
#include "chaparral/executer/invokable.h"

class Light : public Invokable {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<Invokable>* object);

  Light(const std::shared_ptr<const Point3>& position,
        const std::shared_ptr<const Color>& color);
  virtual ~Light();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var);

  const Point3* position() const { return position_.get(); }
  const Color* color() const { return color_.get(); }

 private:
  const std::shared_ptr<const Point3> position_;
  const std::shared_ptr<const Color> color_;

  DISALLOW_COPY_AND_ASSIGN(Light);
};

#endif

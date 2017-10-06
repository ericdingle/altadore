#ifndef SHADER_LIGHT_H_
#define SHADER_LIGHT_H_

#include <memory>
#include "algebra/point3.h"
#include "shader/color.h"
#include "third_party/chaparral/src/executer/invokable.h"

class Light : public Invokable {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  Light(const std::shared_ptr<const Point3>& position,
        const std::shared_ptr<const Color>& color);
  Light(const Light&) = delete;
  Light& operator=(const Light&) = delete;
  virtual ~Light();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  const Point3* position() const { return position_.get(); }
  const Color* color() const { return color_.get(); }

 private:
  const std::shared_ptr<const Point3> position_;
  const std::shared_ptr<const Color> color_;
};

#endif  // SHADER_LIGHT_H_

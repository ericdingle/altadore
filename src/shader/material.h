#ifndef SHADER_MATERIAL_H_
#define SHADER_MATERIAL_H_

#include "shader/color.h"
#include "third_party/chaparral/src/executer/invokable.h"

class Material : public Invokable {
 public:
  static Result Create(
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<Invokable>* object);

  Material(const std::shared_ptr<const Color>& color, double shininess,
           double reflectivity);
  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;
  virtual ~Material();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  const Color* color() const { return color_.get(); }
  double shininess() const { return shininess_; }
  double reflectivity() const { return reflectivity_; }

 private:
  const std::shared_ptr<const Color> color_;
  const double shininess_;
  const double reflectivity_;
};

#endif  // SHADER_MATERIAL_H_

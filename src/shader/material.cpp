#include "shader/material.h"

#include "third_party/chaparral/src/executer/variant.h"

Invokable::Result Material::Create(
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<Invokable>* object) {
  DCHECK(object);

  if (args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  std::shared_ptr<Invokable> color_object;
  if (!args[0]->Get(&color_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Color> color = std::dynamic_pointer_cast<Color>(color_object);
  if (!color.get())
    return RESULT_ERR_ARG_TYPE;

  double shininess, reflectivity;
  if (!args[1]->Get(&shininess) || !args[2]->Get(&reflectivity))
    return RESULT_ERR_ARG_TYPE;

  object->reset(new Material(color, shininess, reflectivity));
  return RESULT_OK;
}

Material::Material(const std::shared_ptr<const Color>& color, double shininess,
                   double reflectivity)
    : color_(color), shininess_(shininess), reflectivity_(reflectivity) {
  DCHECK(reflectivity >= 0 && reflectivity <= 1);
}

Material::~Material() {
}

Invokable::Result Material::Invoke(
    const std::string& name,
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<const Variant>* var) {
  return RESULT_ERR_NAME;
}

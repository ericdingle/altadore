#include "altadore/shader/light_vector.h"

#include "chaparral/executer/variant.h"

LightVector::LightVector() {
}

LightVector::~LightVector() {
}

Invokable::Result LightVector::Invoke(
    const std::string& name,
    const std::vector<std::shared_ptr<const Variant> >& args,
    std::shared_ptr<const Variant>* var) {
  DCHECK(var);

  if (name != "AddLight")
    return RESULT_ERR_NAME;

  if (args.size() != 1)
    return RESULT_ERR_ARG_SIZE;

  std::shared_ptr<Invokable> light_object;
  if (!args[0]->Get(&light_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Light> light = std::dynamic_pointer_cast<Light>(light_object);
  if (!light.get())
    return RESULT_ERR_ARG_TYPE;

  AddLight(light);
  return RESULT_OK;
}

void LightVector::AddLight(const std::shared_ptr<const Light>& light) {
  lights_.push_back(light);
}

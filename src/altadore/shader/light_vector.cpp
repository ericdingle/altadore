#include "altadore/shader/light_vector.h"

#include "chaparral/executer/variant.h"

LightVector::LightVector() {
}

LightVector::~LightVector() {
}

Invokable::Result LightVector::Invoke(
    const std::string& name,
    const std::vector<scoped_refptr<const Variant> >& args,
    const Variant** var) {
  if (name != "AddLight")
    return RESULT_ERR_NAME;

  if (args.size() != 1)
    return RESULT_ERR_ARG_SIZE;

  scoped_refptr<Invokable> light_object;
  if (!args[0]->Get(light_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Light* light = dynamic_cast<Light*>(light_object.ptr());
  if (!light)
    return RESULT_ERR_ARG_TYPE;

  AddLight(light);
  return RESULT_OK;
}

void LightVector::AddLight(const Light* light) {
  lights_.push_back(light);
}

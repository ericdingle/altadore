#include "altadore/shader/material.h"

#include "chaparral/executer/variant.h"

Invokable::Result Material::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  memory::scoped_refptr<Invokable> color_object;
  if (!args[0]->Get(color_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Color* color = dynamic_cast<Color*>(color_object.ptr());
  if (!color)
    return RESULT_ERR_ARG_TYPE;

  double shininess, reflectivity;
  if (!args[1]->Get(&shininess) || !args[2]->Get(&reflectivity))
    return RESULT_ERR_ARG_TYPE;

  memory::scoped_refptr<Material> material(
      new Material(color, shininess, reflectivity));
  *object = material.Release();
  return RESULT_OK;
}

Material::Material(const Color* color, double shininess, double reflectivity)
    : color_(color), shininess_(shininess), reflectivity_(reflectivity) {
  ASSERT(color);
  ASSERT(reflectivity >= 0 && reflectivity <= 1);
}

Material::~Material() {
}

Invokable::Result Material::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

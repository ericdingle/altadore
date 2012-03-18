#include "altadore/shader/light.h"

#include "chaparral/executer/variant.h"

Invokable::Result Light::Create(
    const std::vector<scoped_refptr<const Variant> >& args,
    Invokable** object) {
  DCHECK(object);

  if (args.size() != 2)
    return RESULT_ERR_ARG_SIZE;

  scoped_refptr<Invokable> position_object;
  if (!args[0]->Get(position_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Point3* position = dynamic_cast<Point3*>(position_object.ptr());
  if (!position)
    return RESULT_ERR_ARG_TYPE;

  scoped_refptr<Invokable> color_object;
  if (!args[1]->Get(color_object.Receive()))
    return RESULT_ERR_ARG_TYPE;
  Color* color = dynamic_cast<Color*>(color_object.ptr());
  if (!color)
    return RESULT_ERR_ARG_TYPE;

  scoped_refptr<Light> light(new Light(position, color));
  *object = light.Release();
  return RESULT_OK;
}

Light::Light(const Point3* position, const Color* color) : position_(position), color_(color) {
  DCHECK(position);
  DCHECK(color);
}

Light::~Light() {
}

Invokable::Result Light::Invoke(
    const std::string& name,
    const std::vector<scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

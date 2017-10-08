#include "shader/light.h"

#include "third_party/chaparral/src/executer/variant.h"

Invokable::Result Light::Create(
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<Invokable>* object) {
  if (args.size() != 2)
    return RESULT_ERR_ARG_SIZE;

  std::shared_ptr<Invokable> position_object;
  if (!args[0]->Get(&position_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Point3> position = std::dynamic_pointer_cast<Point3>(position_object);
  if (!position.get())
    return RESULT_ERR_ARG_TYPE;

  std::shared_ptr<Invokable> color_object;
  if (!args[1]->Get(&color_object))
    return RESULT_ERR_ARG_TYPE;
  std::shared_ptr<Color> color = std::dynamic_pointer_cast<Color>(color_object);
  if (!color.get())
    return RESULT_ERR_ARG_TYPE;

  object->reset(new Light(position, color));
  return RESULT_OK;
}

Light::Light(const std::shared_ptr<const Point3>& position,
             const std::shared_ptr<const Color>& color)
    : position_(position), color_(color) {
}

Light::~Light() {
}

Invokable::Result Light::Invoke(
    const std::string& name,
    const std::vector<std::shared_ptr<const Variant>>& args,
    std::shared_ptr<const Variant>* var) {
  return RESULT_ERR_NAME;
}

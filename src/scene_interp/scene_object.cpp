#include "scene_interp/scene_object.h"

StatusOr<Any> SceneObject::Get(const std::string& name) {
  return Status(name + " is undefined", 1, 1);
}

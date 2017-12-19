#include "scene_interp/scene_object.h"

StatusOr<Any> SceneObject::Get(const Token& token) {
  return Status(token.value() + " is undefined", token.line(), token.column());
}

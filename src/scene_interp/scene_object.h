#ifndef EXECUTER_SCENE_OBJECT_H_
#define EXECUTER_SCENE_OBJECT_H_

#include <string>
#include "third_party/bonavista/src/util/status_or.h"
#include "third_party/chaparral/src/executer/any.h"

class SceneObject {
 public:
  SceneObject() = default;
  SceneObject(const SceneObject&) = delete;
  SceneObject& operator=(const SceneObject&) = delete;
  virtual ~SceneObject() = default;

  virtual StatusOr<Any> Get(const std::string& name) = 0;
};

#endif  // EXECUTER_SCENE_OBJECT_H_

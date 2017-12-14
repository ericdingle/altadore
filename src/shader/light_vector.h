#ifndef SHADER_LIGHT_VECTOR_H_
#define SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "shader/light.h"

class LightVector {
 public:
  LightVector() = default;
  LightVector(const LightVector&) = delete;
  LightVector& operator=(const LightVector&) = delete;
  ~LightVector() = default;

  void AddLight(const std::shared_ptr<const Light>& light);

  const std::vector<std::shared_ptr<const Light>>& lights() const { return lights_; }

 private:
  std::vector<std::shared_ptr<const Light>> lights_;
};

#endif  // SHADER_LIGHT_VECTOR_H_

#include "shader/light_vector.h"

void LightVector::AddLight(const std::shared_ptr<const Light>& light) {
  lights_.push_back(light);
}

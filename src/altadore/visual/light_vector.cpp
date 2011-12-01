#include "altadore/visual/light_vector.h"

LightVector::LightVector() {
}

LightVector::~LightVector() {
}

void LightVector::AddLight(const Light* light) {
  lights_.push_back(light);
}

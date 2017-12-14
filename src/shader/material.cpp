#include "shader/material.h"

#include <assert.h>

Material::Material(const std::shared_ptr<const Color>& color, double shininess,
                   double reflectivity)
    : color_(color), shininess_(shininess), reflectivity_(reflectivity) {
  assert(reflectivity >= 0 && reflectivity <= 1);
}

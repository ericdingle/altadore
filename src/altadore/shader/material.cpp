#include "altadore/shader/material.h"

Material::Material(const Color* color, double shininess, double reflectivity) : color_(color), shininess_(shininess), reflectivity_(reflectivity) {
  ASSERT(color);
  ASSERT(reflectivity >= 0 && reflectivity <= 1);
}

Material::~Material() {
}

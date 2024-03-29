#include "altadore/algebra/vector3.h"

#include <assert.h>
#include <math.h>
#include <string.h>

Vector3::Vector3() {
  v_[0] = 0.0;
  v_[1] = 0.0;
  v_[2] = 0.0;
}

Vector3::Vector3(double x, double y, double z) {
  v_[0] = x;
  v_[1] = y;
  v_[2] = z;
}

Vector3::~Vector3() {
}

Vector3::Vector3(const Vector3& v) {
  *this = v;
}

Vector3& Vector3::operator=(const Vector3& v) {
  v_[0] = v.v_[0];
  v_[1] = v.v_[1];
  v_[2] = v.v_[2];

  return *this;
}

double& Vector3::operator[](int i) {
  assert(i <= 2);
  return v_[i];
}

double Vector3::operator[](int i) const {
  assert(i <= 2);
  return v_[i];
}

Vector3 Vector3::operator*(double s) const {
  return Vector3(s*v_[0], s*v_[1], s*v_[2]);
}

Vector3 Vector3::operator+(const Vector3& v) const {
  return Vector3(v_[0]+v.v_[0], v_[1]+v.v_[1], v_[2]+v.v_[2]);
}

Vector3 Vector3::operator-() const {
  return Vector3(-v_[0], -v_[1], -v_[2]);
}

Vector3 Vector3::operator-(const Vector3& v) const {
  return Vector3(v_[0]-v.v_[0], v_[1]-v.v_[1], v_[2]-v.v_[2]);
}

Vector3 Vector3::Cross(const Vector3& v) const {
  return Vector3(v_[1]*v.v_[2] - v_[2]*v.v_[1],
                 v_[2]*v.v_[0] - v_[0]*v.v_[2],
                 v_[0]*v.v_[1] - v_[1]*v.v_[0]);
}

double Vector3::Dot(const Vector3& v) const {
  return v_[0]*v.v_[0] + v_[1]*v.v_[1] + v_[2]*v.v_[2];
}

double Vector3::Length() const {
  return sqrt(v_[0]*v_[0] + v_[1]*v_[1] + v_[2]*v_[2]);
}

void Vector3::Normalize() {
  double len = Length();
  v_[0] /= len;
  v_[1] /= len;
  v_[2] /= len;
}

bool Vector3::operator==(const Vector3& v) const {
  return memcmp(v_, v.v_, 3 * sizeof(double)) == 0;
}

#include "algebra/point3.h"

#include <assert.h>
#include <string.h>
#include "algebra/vector3.h"

Point3::Point3() {
  p_[0] = 0.0;
  p_[1] = 0.0;
  p_[2] = 0.0;
}

Point3::Point3(double x, double y, double z) {
  p_[0] = x;
  p_[1] = y;
  p_[2] = z;
}

Point3::Point3(const Point3& p) {
  *this = p;
}

Point3& Point3::operator=(const Point3& p) {
  p_[0] = p.p_[0];
  p_[1] = p.p_[1];
  p_[2] = p.p_[2];

  return *this;
}

double& Point3::operator[](int i) {
  assert(i <= 2);
  return p_[i];
}

double Point3::operator[](int i) const {
  assert(i <= 2);
  return p_[i];
}

Point3 Point3::operator+(const Vector3& v) const {
  return Point3(p_[0]+v[0], p_[1]+v[1], p_[2]+v[2]);
}

Vector3 Point3::operator-(const Point3& p) const {
  return Vector3(p_[0]-p.p_[0], p_[1]-p.p_[1], p_[2]-p.p_[2]);
}

bool Point3::operator==(const Point3& p) const {
  return memcmp(p_, p.p_, 3 * sizeof(double)) == 0;
}

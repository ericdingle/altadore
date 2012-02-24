#include "altadore/algebra/point3.h"

#include <string.h>
#include "altadore/algebra/vector3.h"
#include "bonavista/logging/assert.h"
#include "chaparral/executer/variant.h"

Invokable::Result Point3::Create(
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    Invokable** object) {
  ASSERT(object);

  if (args.size() != 0 && args.size() != 3)
    return RESULT_ERR_ARG_SIZE;

  if (args.size() == 0) {
    *object = new Point3();
    return RESULT_OK;
  }

  double x;
  if (!args[0]->Get(&x))
    return RESULT_ERR_ARG_TYPE;
  double y;
  if (!args[1]->Get(&y))
    return RESULT_ERR_ARG_TYPE;
  double z;
  if (!args[2]->Get(&z))
    return RESULT_ERR_ARG_TYPE;

  *object = new Point3(x, y, z);
  return RESULT_OK;
}

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

Point3::~Point3() {
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

Invokable::Result Point3::Invoke(
    const std::string& name,
    const std::vector<memory::scoped_refptr<const Variant> >& args,
    const Variant** var) {
  return RESULT_ERR_NAME;
}

double& Point3::operator[](uint i) {
  DASSERT(i <= 2);
  return p_[i];
}

double Point3::operator[](uint i) const {
  DASSERT(i <= 2);
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

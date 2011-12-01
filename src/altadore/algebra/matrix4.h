#ifndef ALTADORE_ALGEBRA_MATRIX4_H_
#define ALTADORE_ALGEBRA_MATRIX4_H_

#include "bonavista/base/types.h"

class Point3;
class Vector3;

class Matrix4 {
public:
  static Matrix4 GetRotation(char axis, double angle);
  static Matrix4 GetScaling(double s);
  static Matrix4 GetScaling(double x, double y, double z);
  static Matrix4 GetTranslation(double x, double y, double z);

  Matrix4();
  Matrix4(double d[4][4]);
  ~Matrix4();

  Matrix4(const Matrix4& m);
  Matrix4& operator=(const Matrix4& m);

  Matrix4 GetInverse() const;
  Matrix4 GetTranspose() const;

  Matrix4 operator*(const Matrix4& m) const;
  Point3 operator*(const Point3& p) const;
  Vector3 operator*(const Vector3& v) const;

  bool operator==(const Matrix4& m) const;

private:
  double m_[4][4];
};

#endif

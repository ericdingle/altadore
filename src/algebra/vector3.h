#ifndef ALGEBRA_VECTOR3_H_
#define ALGEBRA_VECTOR3_H_

class Point3;

class Vector3 {
 public:
  Vector3();
  Vector3(double x, double y, double z);
  ~Vector3();

  Vector3(const Vector3& v);
  Vector3& operator=(const Vector3& v);
  Vector3(const Point3& p);

  double& operator[](uint i);
  double operator[](uint i) const;

  Vector3 operator*(double s) const;
  Vector3 operator+(const Vector3& v) const;
  Vector3 operator-() const;
  Vector3 operator-(const Vector3& v) const;

  Vector3 Cross(const Vector3& v) const;
  double Dot(const Vector3& v) const;

  double Length() const;
  void Normalize();

  bool operator==(const Vector3& v) const;

 private:
  double v_[3];
};

#endif  // ALGEBRA_VECTOR3_H_

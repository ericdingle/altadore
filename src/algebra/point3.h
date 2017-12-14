#ifndef ALGEBRA_POINT3_H_
#define ALGEBRA_POINT3_H_

class Vector3;

class Point3 {
 public:
  Point3();
  Point3(double x, double y, double z);
  ~Point3() = default;

  Point3(const Point3& p);
  Point3& operator=(const Point3& p);

  double& operator[](int i);
  double operator[](int i) const;

  Point3 operator+(const Vector3& v) const;
  Vector3 operator-(const Point3& p) const;

  bool operator==(const Point3& p) const;

 private:
  double p_[3];
};

#endif  // ALGEBRA_POINT3_H_

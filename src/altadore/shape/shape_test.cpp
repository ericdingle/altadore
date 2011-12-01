#include "altadore/shape/shape.h"

#include "bonavista/testing/test_case.h"

class TestShape : public Shape {
public:
  TestShape() : Shape() {}
  ~TestShape() {}

  bool FindIntersection(const Ray& ray, double* t, Point3* point, Vector3* normal) const { return false; }
  bool HasIntersection(const Ray& ray) const { return false; }
};

TEST_CASE(ShapeTest) {
};

TEST(ShapeTest, Constructor) {
  TestShape shape;
}

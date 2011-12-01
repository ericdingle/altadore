#include "altadore/algebra/ray.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(RayTest) {
};

TEST(RayTest, Constructor) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
}

TEST(RayTest, Origin) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
  const Point3& origin = ray.origin();
  EXPECT_EQ(origin[0], 1.0);
  EXPECT_EQ(origin[1], 2.0);
  EXPECT_EQ(origin[2], 3.0);
}

TEST(RayTest, Direction) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
  const Vector3& direction1 = ray.direction();
  EXPECT_EQ(direction1[0], 0.1);
  EXPECT_EQ(direction1[1], 0.2);
  EXPECT_EQ(direction1[2], 0.3);

  ray.set_direction(Vector3(4.0, 5.0, 6.0));
  const Vector3& direction2 = ray.direction();
  EXPECT_EQ(direction2[0], 4.0);
  EXPECT_EQ(direction2[1], 5.0);
  EXPECT_EQ(direction2[2], 6.0);
}

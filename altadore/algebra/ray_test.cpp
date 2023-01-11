#include "altadore/algebra/ray.h"

#include <gtest/gtest.h>

TEST(RayTest, Constructor) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
}

TEST(RayTest, Origin) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
  const Point3& origin = ray.origin();
  EXPECT_EQ(1, origin[0]);
  EXPECT_EQ(2, origin[1]);
  EXPECT_EQ(3, origin[2]);
}

TEST(RayTest, Direction) {
  Ray ray(Point3(1.0, 2.0, 3.0), Vector3(0.1, 0.2, 0.3));
  const Vector3& direction1 = ray.direction();
  EXPECT_EQ(0.1, direction1[0]);
  EXPECT_EQ(0.2, direction1[1]);
  EXPECT_EQ(0.3, direction1[2]);

  ray.set_direction(Vector3(4.0, 5.0, 6.0));
  const Vector3& direction2 = ray.direction();
  EXPECT_EQ(4, direction2[0]);
  EXPECT_EQ(5, direction2[1]);
  EXPECT_EQ(6, direction2[2]);
}

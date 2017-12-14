#include "shape/sphere.h"

#include "algebra/ray.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(SphereTest, FindIntersection) {
  Sphere sphere;
  double t;
  Point3 point;
  Vector3 normal;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.FindIntersection(ray1, &t, &point, &normal));
  EXPECT_EQ(1, t);
  EXPECT_EQ(0, point[0]);
  EXPECT_EQ(0, point[1]);
  EXPECT_EQ(1, point[2]);
  EXPECT_EQ(0, normal[0]);
  EXPECT_EQ(0, normal[1]);
  EXPECT_EQ(1, normal[2]);

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.FindIntersection(ray2, &t, &point, &normal));
}

TEST(SphereTest, HasIntersection) {
  Sphere sphere;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.HasIntersection(ray1, 2));
  EXPECT_FALSE(sphere.HasIntersection(ray1, 0.5));

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.HasIntersection(ray2, 20));
}

#include "altadore/shape/sphere.h"

#include "altadore/algebra/ray.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(SphereTest) {
};

TEST(SphereTest, FindIntersection) {
  Sphere sphere;
  double t;
  Point3 point;
  Vector3 normal;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.FindIntersection(ray1, &t, &point, &normal));
  EXPECT_EQ(t, 1);
  EXPECT_EQ(point[0], 0);
  EXPECT_EQ(point[1], 0);
  EXPECT_EQ(point[2], 1);
  EXPECT_EQ(normal[0], 0);
  EXPECT_EQ(normal[1], 0);
  EXPECT_EQ(normal[2], 1);

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.FindIntersection(ray2, &t, &point, &normal));
}

TEST(SphereTest, HasIntersection) {
  Sphere sphere;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.HasIntersection(ray1));

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.HasIntersection(ray2));
}

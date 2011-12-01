#include "altadore/shape/cube.h"

#include "altadore/algebra/ray.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(CubeTest) {
};

TEST(CubeTest, FindIntersection) {
  Cube cube;
  double t;
  Point3 point;
  Vector3 normal;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(cube.FindIntersection(ray1, &t, &point, &normal));
  EXPECT_EQ(t, 1);
  EXPECT_EQ(point[0], 0);
  EXPECT_EQ(point[1], 0);
  EXPECT_EQ(point[2], 1);
  EXPECT_EQ(normal[0], 0);
  EXPECT_EQ(normal[1], 0);
  EXPECT_EQ(normal[2], 1);

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(cube.FindIntersection(ray2, &t, &point, &normal));
}

TEST(CubeTest, HasIntersection) {
  Cube cube;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(cube.HasIntersection(ray1));

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(cube.HasIntersection(ray2));
}

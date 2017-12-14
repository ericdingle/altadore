#include "shape/cube.h"

#include "algebra/ray.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(CubeTest, FindIntersection) {
  Cube cube;
  double t;
  Point3 point;
  Vector3 normal;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(cube.FindIntersection(ray1, &t, &point, &normal));
  EXPECT_EQ(1, t);
  EXPECT_EQ(0, point[0]);
  EXPECT_EQ(0, point[1]);
  EXPECT_EQ(1, point[2]);
  EXPECT_EQ(0, normal[0]);
  EXPECT_EQ(0, normal[1]);
  EXPECT_EQ(1, normal[2]);

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(cube.FindIntersection(ray2, &t, &point, &normal));
}

TEST(CubeTest, HasIntersection) {
  Cube cube;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(cube.HasIntersection(ray1, 2));
  EXPECT_FALSE(cube.HasIntersection(ray1, 0.5));

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(cube.HasIntersection(ray2, 20));
}

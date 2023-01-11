#include "altadore/algebra/point3.h"

#include <gtest/gtest.h>

#include "altadore/algebra/vector3.h"

TEST(Point3Test, Constructor) {
  Point3 point1;
  EXPECT_EQ(0, point1[0]);
  EXPECT_EQ(0, point1[1]);
  EXPECT_EQ(0, point1[2]);

  Point3 point2(1.0, 2.0, 3.0);
  EXPECT_EQ(1, point2[0]);
  EXPECT_EQ(2, point2[1]);
  EXPECT_EQ(3, point2[2]);
}

TEST(Point3Test, CopyConstructor) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2(point1);
  EXPECT_TRUE(point1 == point2);
}

TEST(Point3Test, AssignmentOperator) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2;
  point2 = point1;
  EXPECT_TRUE(point1 == point2);
}

TEST(Point3Test, IndexOperator) {
  Point3 point(1.0, 2.0, 3.0);
  point[0] = 4.0;
  point[1] = 5.0;
  point[2] = 6.0;
  EXPECT_TRUE(point == Point3(4.0, 5.0, 6.0));
}

TEST(Point3Test, VectorAddition) {
  Point3 point1(1.0, 2.0, 3.0);
  Vector3 vector(4.0, 5.0, 6.0);
  Point3 point2(point1 + vector);
  EXPECT_TRUE(point2 == Point3(5.0, 7.0, 9.0));
}

TEST(Point3Test, PointSubtraction) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2(6.0, 5.0, 4.0);
  Vector3 vector(point2 - point1);
  EXPECT_TRUE(vector == Vector3(5.0, 3.0, 1.0));
}

TEST(Vector3Test, ToVector3) {
  Point3 point(1.0, 2.0, 3.0);
  Vector3 vector = point.ToVector3();
  EXPECT_TRUE(vector == Vector3(1.0, 2.0, 3.0));
}

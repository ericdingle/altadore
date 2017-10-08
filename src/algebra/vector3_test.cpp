#include "algebra/vector3.h"

#include "algebra/point3.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(Vector3Test, Constructor) {
  Vector3 vector1;
  EXPECT_EQ(0, vector1[0]);
  EXPECT_EQ(0, vector1[1]);
  EXPECT_EQ(0, vector1[2]);

  Vector3 vector2(1.0, 2.0, 3.0);
  EXPECT_EQ(1, vector2[0]);
  EXPECT_EQ(2, vector2[1]);
  EXPECT_EQ(3, vector2[2]);
}

TEST(Vector3Test, CopyConstructorVector3) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector2(vector1);
  EXPECT_TRUE(vector1 == vector2);
}

TEST(Vector3Test, AssignmentOperator) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector2;
  vector2 = vector1;
  EXPECT_TRUE(vector1 == vector2);
}

TEST(Vector3Test, CopyConstructorPoint3) {
  Point3 point(1.0, 2.0, 3.0);
  Vector3 vector(point);
  EXPECT_TRUE(vector == Vector3(1.0, 2.0, 3.0));
}

TEST(Vector3Test, IndexOperator) {
  Vector3 vector(1.0, 2.0, 3.0);
  vector[0] = 4.0;
  vector[1] = 5.0;
  vector[2] = 6.0;
  EXPECT_TRUE(vector == Vector3(4.0, 5.0, 6.0));
}

TEST(Vector3Test, ScalarMultiplication) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector2(vector1 * 2.5);
  EXPECT_TRUE(vector2 == Vector3(2.5, 5.0, 7.5));
}

TEST(Vector3Test, VectorAddition) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector(4.0, 5.0, 6.0);
  Vector3 vector2(vector1 + vector);
  EXPECT_TRUE(vector2 == Vector3(5.0, 7.0, 9.0));
}

TEST(Vector3Test, Negation) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector2(-vector1);
  EXPECT_TRUE(vector2 == Vector3(-1.0, -2.0, -3.0));
}

TEST(Vector3Test, VectorSubtraction) {
  Vector3 vector1(1.0, 2.0, 3.0);
  Vector3 vector2(6.0, 5.0, 4.0);
  Vector3 vector3(vector2 - vector1);
  EXPECT_TRUE(vector3 == Vector3(5.0, 3.0, 1.0));
}

TEST(Vector3Test, CrossProduct) {
  Vector3 vector1(3, -3, 1);
  Vector3 vector2(4, 9, 2);
  Vector3 vector3(vector1.Cross(vector2));
  EXPECT_TRUE(vector3 == Vector3(-15, -2, 39));
}

TEST(Vector3Test, DotProduct) {
  Vector3 vector1(2, 4, 8);
  Vector3 vector2(1, 5, -1);
  EXPECT_EQ(14, vector1.Dot(vector2));
}

TEST(Vector3Test, Length) {
  Vector3 vector(9, 2, 6);
  EXPECT_EQ(11, vector.Length());
}

TEST(Vector3Test, Normalize) {
  Vector3 vector1(9, 2, 6);
  Vector3 vector2(vector1);
  vector2.Normalize();
  EXPECT_EQ(1, vector2.Length());
  EXPECT_EQ(vector1[0] / vector2[0], vector1[1] / vector2[1]);
}

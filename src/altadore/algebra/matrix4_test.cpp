#include "altadore/algebra/matrix4.h"

#include <math.h>
#include "altadore/algebra/point3.h"
#include "altadore/algebra/vector3.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(Matrix4Test) {
};

TEST(Matrix4Test, GetRotation) {
  double x[][4] = {
    {1, 0,           0,            0},
    {0, 0.5,         -sqrt(3.0)/2, 0},
    {0, sqrt(3.0)/2, 0.5,          0},
    {0, 0,           0,            1}
  };
  EXPECT_TRUE(Matrix4::GetRotation(Matrix4::AXIS_X, 60) == x);

  double y[][4] = {
    {0.5,          0, sqrt(3.0)/2, 0},
    {0,            1, 0,           0},
    {-sqrt(3.0)/2, 0, 0.5,         0},
    {0,            0, 0,           1}
  };
  EXPECT_TRUE(Matrix4::GetRotation(Matrix4::AXIS_Y, 60) == y);

  double z[][4] = {
    {0.5,         -sqrt(3.0)/2, 0, 0},
    {sqrt(3.0)/2, 0.5,          0, 0},
    {0,           0,            1, 0},
    {0,           0,            0, 1}
  };
  EXPECT_TRUE(Matrix4::GetRotation(Matrix4::AXIS_Z, 60) == z);
}

TEST(Matrix4Test, GetScaling) {
  double d1[][4] = {
    {4, 0, 0, 0},
    {0, 4, 0, 0},
    {0, 0, 4, 0},
    {0, 0, 0, 1}
  };
  EXPECT_TRUE(Matrix4::GetScaling(4) == d1);

  double d2[][4] = {
    {4, 0, 0, 0},
    {0, 3, 0, 0},
    {0, 0, 2, 0},
    {0, 0, 0, 1}
  };
  EXPECT_TRUE(Matrix4::GetScaling(4, 3, 2) == d2);
}

TEST(Matrix4Test, GetTranslation) {
  double d[][4] = {
    {1, 0, 0, 1},
    {0, 1, 0, 2},
    {0, 0, 1, 3},
    {0, 0, 0, 1}
  };
  EXPECT_TRUE(Matrix4::GetTranslation(1, 2, 3) == d);
}

TEST(Matrix4Test, Constructor) {
  double d[][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };

  Matrix4 m;
  EXPECT_TRUE(m == d);
}

TEST(Matrix4Test, ConstructorDouble) {
  double d[][4] = {
    {1, 0, 0, 1},
    {0, 1, 0, 2},
    {0, 0, 1, 3},
    {0, 0, 0, 1}
  };
  EXPECT_TRUE(Matrix4(d) == d);
}

TEST(Matrix4Test, CopyConstructor) {
  double d[][4] = {
    {1, 0, 0, 1},
    {0, 1, 0, 2},
    {0, 0, 1, 3},
    {0, 0, 0, 1}
  };
  Matrix4 m1(d);
  Matrix4 m2(m1);
  EXPECT_TRUE(m2 == d);
}

TEST(Matrix4Test, AssignmentOperator) {
  double d[][4] = {
    {1, 0, 0, 1},
    {0, 1, 0, 2},
    {0, 0, 1, 3},
    {0, 0, 0, 1}
  };
  Matrix4 m1(d);
  Matrix4 m2;
  m2 = m1;
  EXPECT_TRUE(m2 == d);
}

TEST(Matrix4Test, GetInverse) {
  Matrix4 m = Matrix4::GetRotation(Matrix4::AXIS_X, 78);
  Matrix4 inv = m.GetInverse();
  EXPECT_TRUE(m * inv == Matrix4());
}

TEST(Matrix4Test, GetTranspose) {
  Matrix4 a = Matrix4::GetRotation(Matrix4::AXIS_X, 34);
  Matrix4 b = Matrix4::GetRotation(Matrix4::AXIS_Y, 83);

  EXPECT_TRUE(a.GetTranspose().GetTranspose() == a);
  EXPECT_TRUE((a * b).GetTranspose() == b.GetTranspose() * a.GetTranspose());
}

TEST(Matrix4Test, MatrixMultiplication) {
  double d1[][4] = {
    {1, 2, 3, 4},
    {4, 3, 2, 1},
    {5, 6, 7, 8},
    {8, 7, 6, 5},
  };

  double d2[][4] = {
    {3, 0, 1, 3},
    {9, 4, 4, 1},
    {4, 6, 7, 1},
    {1, 4, 6, 7},
  };

  double result[][4] = {
    {37, 42, 54, 36},
    {48, 28, 36, 24},
    {105, 98, 126, 84},
    {116, 84, 108, 72},
  };

  EXPECT_TRUE(Matrix4(d1) * d2 == result);
}

TEST(Matrix4Test, PointMultiplication) {
  double d[][4] = {
    {1, 2, 3, 4},
    {4, 3, 2, 1},
    {5, 6, 7, 8},
    {8, 7, 6, 5},
  };

  Point3 result = Matrix4(d) * Point3(1, 2, 3);
  EXPECT_EQ(result[0], 18);
  EXPECT_EQ(result[1], 17);
  EXPECT_EQ(result[2], 46);
}

TEST(Matrix4Test, VectorMultiplication) {
  double d[][4] = {
    {1, 2, 3, 4},
    {4, 3, 2, 1},
    {5, 6, 7, 8},
    {8, 7, 6, 5},
  };

  Vector3 result = Matrix4(d) * Vector3(1, 2, 3);
  EXPECT_EQ(result[0], 14);
  EXPECT_EQ(result[1], 16);
  EXPECT_EQ(result[2], 38);
}

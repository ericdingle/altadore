#include "altadore/shader/material.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(MaterialTest) {
};

TEST(MaterialTest, Constructor) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
}

TEST(MaterialTest, Color) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  const Color* color = mat.color();
  EXPECT_EQ(color->r(), 0.1);
  EXPECT_EQ(color->g(), 0.2);
  EXPECT_EQ(color->b(), 0.3);
}

TEST(MaterialTest, Shininess) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  double shininess = mat.shininess();
  EXPECT_EQ(shininess, 25);
}

TEST(MaterialTest, Reflectivity) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  double reflectivity = mat.reflectivity();
  EXPECT_EQ(reflectivity, 0.4);
}

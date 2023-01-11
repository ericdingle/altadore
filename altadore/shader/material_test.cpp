#include "altadore/shader/material.h"

#include <gtest/gtest.h>

TEST(MaterialTest, Constructor) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
}

TEST(MaterialTest, Color) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  const Color* color = mat.color();
  EXPECT_EQ(0.1, color->r());
  EXPECT_EQ(0.2, color->g());
  EXPECT_EQ(0.3, color->b());
}

TEST(MaterialTest, Shininess) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  double shininess = mat.shininess();
  EXPECT_EQ(25, shininess);
}

TEST(MaterialTest, Reflectivity) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  double reflectivity = mat.reflectivity();
  EXPECT_EQ(0.4, reflectivity);
}

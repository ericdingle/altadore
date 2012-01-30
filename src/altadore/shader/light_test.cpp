#include "altadore/shader/light.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(LightTest) {
};

TEST(LightTest, Constructor) {
  Light light(new Point3(1.0, 2.0, 3.0), new Color(0.1, 0.2, 0.3));
}

TEST(LightTest, Position) {
  Light light(new Point3(1.0, 2.0, 3.0), new Color());
  const Point3* point = light.position();
  EXPECT_EQ((*point)[0], 1.0);
  EXPECT_EQ((*point)[1], 2.0);
  EXPECT_EQ((*point)[2], 3.0);
}

TEST(LightTest, Color) {
  Light light(new Point3(), new Color(0.1, 0.2, 0.3));
  const Color* color = light.color();
  EXPECT_EQ(color->r(), 0.1);
  EXPECT_EQ(color->g(), 0.2);
  EXPECT_EQ(color->b(), 0.3);
}

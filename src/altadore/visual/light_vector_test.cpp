#include "altadore/visual/light_vector.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(LightVectorTest) {
};

TEST(LightVectorTest, Constructor) {
  LightVector lights;
  EXPECT_EQ(lights.lights().size(), 0);
}

TEST(LightVectorTest, AddLight) {
  LightVector lights;
  lights.AddLight(new Light(new Point3(), new Color()));
  EXPECT_EQ(lights.lights().size(), 1);
}

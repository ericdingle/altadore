#include "shader/light_vector.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(LightVectorTest, Constructor) {
  LightVector lights;
  EXPECT_EQ(0, lights.lights().size());
}

TEST(LightVectorTest, AddLight) {
  LightVector lights;
  lights.AddLight(std::make_shared<Light>(std::make_shared<Point3>(),
                                          std::make_shared<Color>()));
  EXPECT_EQ(1, lights.lights().size());
}

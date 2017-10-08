#include "shader/light_vector.h"

#include "third_party/chaparral/src/executer/variant.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(LightVectorTest, Invoke) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Light(std::make_shared<Point3>(1.0, 2.0, 3.0),
                         std::make_shared<Color>(0.1, 0.2, 0.3)));
  var.reset(new Variant(object));
  args.push_back(var);

  LightVector lights;
  EXPECT_EQ(Invokable::RESULT_OK, lights.Invoke("AddLight", args, &var));
  EXPECT_NE(nullptr, object.get());
}

TEST(LightVectorTest, InvokeError) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<const Variant> var;

  LightVector lights;
  EXPECT_EQ(Invokable::RESULT_ERR_NAME, lights.Invoke("blah", args, &var));
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE,
            lights.Invoke("AddLight", args, &var));

  var.reset(new Variant(2.0));
  args.push_back(var);

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE,
            lights.Invoke("AddLight", args, &var));
}

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

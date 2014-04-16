#include "altadore/shader/light_vector.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(LightVectorTest) {
};

TEST(LightVectorTest, Invoke) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Light(std::make_shared<Point3>(1.0, 2.0, 3.0),
                         std::make_shared<Color>(0.1, 0.2, 0.3)));
  var.reset(new Variant(object));
  args.push_back(var);

  LightVector lights;
  EXPECT_EQ(lights.Invoke("AddLight", args, &var), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());
}

TEST(LightVectorTest, InvokeError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var;

  LightVector lights;
  EXPECT_EQ(lights.Invoke("blah", args, &var),
            Invokable::RESULT_ERR_NAME);

  EXPECT_EQ(lights.Invoke("AddLight", args, &var),
            Invokable::RESULT_ERR_ARG_SIZE);

  var.reset(new Variant(2.0));
  args.push_back(var);

  EXPECT_EQ(lights.Invoke("AddLight", args, &var),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(LightVectorTest, Constructor) {
  LightVector lights;
  EXPECT_EQ(lights.lights().size(), 0);
}

TEST(LightVectorTest, AddLight) {
  LightVector lights;
  lights.AddLight(std::make_shared<Light>(std::make_shared<Point3>(),
                                          std::make_shared<Color>()));
  EXPECT_EQ(lights.lights().size(), 1);
}

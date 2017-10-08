#include "shader/light.h"

#include "third_party/chaparral/src/executer/variant.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(LightTest, Create) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Point3(0.1, 0.2, 0.3));
  var.reset(new Variant(object));
  args.push_back(var);

  object.reset(new Color(0.1, 0.2, 0.3));
  var.reset(new Variant(object));
  args.push_back(var);

  EXPECT_EQ(Invokable::RESULT_OK, Light::Create(args, &object));
  EXPECT_NE(nullptr, object.get());
}

TEST(LightTest, CreateError) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE, Light::Create(args, &object));

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE, Light::Create(args, &object));
}

TEST(LightTest, Constructor) {
  Light light(std::make_shared<Point3>(1.0, 2.0, 3.0),
              std::make_shared<Color>(0.1, 0.2, 0.3));
}

TEST(LightTest, Position) {
  Light light(std::make_shared<Point3>(1.0, 2.0, 3.0),
              std::make_shared<Color>());
  const Point3* point = light.position();
  EXPECT_EQ(1, (*point)[0]);
  EXPECT_EQ(2, (*point)[1]);
  EXPECT_EQ(3, (*point)[2]);
}

TEST(LightTest, Color) {
  Light light(std::make_shared<Point3>(),
              std::make_shared<Color>(0.1, 0.2, 0.3));
  const Color* color = light.color();
  EXPECT_EQ(0.1, color->r());
  EXPECT_EQ(0.2, color->g());
  EXPECT_EQ(0.3, color->b());
}

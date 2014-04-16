#include "altadore/shader/light.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(LightTest) {
};

TEST(LightTest, Create) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Point3(0.1, 0.2, 0.3));
  var.reset(new Variant(object));
  args.push_back(var);

  object.reset(new Color(0.1, 0.2, 0.3));
  var.reset(new Variant(object));
  args.push_back(var);

  EXPECT_EQ(Light::Create(args, &object), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());
}

TEST(LightTest, CreateError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Light::Create(args, &object),
            Invokable::RESULT_ERR_ARG_SIZE);

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(Light::Create(args, &object),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(LightTest, Constructor) {
  Light light(std::make_shared<Point3>(1.0, 2.0, 3.0),
              std::make_shared<Color>(0.1, 0.2, 0.3));
}

TEST(LightTest, Position) {
  Light light(std::make_shared<Point3>(1.0, 2.0, 3.0),
              std::make_shared<Color>());
  const Point3* point = light.position();
  EXPECT_EQ((*point)[0], 1.0);
  EXPECT_EQ((*point)[1], 2.0);
  EXPECT_EQ((*point)[2], 3.0);
}

TEST(LightTest, Color) {
  Light light(std::make_shared<Point3>(),
              std::make_shared<Color>(0.1, 0.2, 0.3));
  const Color* color = light.color();
  EXPECT_EQ(color->r(), 0.1);
  EXPECT_EQ(color->g(), 0.2);
  EXPECT_EQ(color->b(), 0.3);
}

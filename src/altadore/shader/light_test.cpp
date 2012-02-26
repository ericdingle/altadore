#include "altadore/shader/light.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(LightTest) {
};

TEST(LightTest, Create) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_refptr<const Variant> var;
  memory::scoped_refptr<Invokable> object;

  object.Reset(new Point3(0.1, 0.2, 0.3));
  var.Reset(new Variant(object.ptr()));
  args.push_back(var.ptr());

  object.Reset(new Color(0.1, 0.2, 0.3));
  var.Reset(new Variant(object.ptr()));
  args.push_back(var.ptr());

  EXPECT_EQ(Light::Create(args, object.Receive()), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.ptr());
}

TEST(LightTest, CreateError) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_ptr<Invokable> object;
  EXPECT_EQ(Light::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  memory::scoped_refptr<const Variant> var(new Variant(1.0));
  args.push_back(var.ptr());
  args.push_back(var.ptr());

  EXPECT_EQ(Light::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}
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

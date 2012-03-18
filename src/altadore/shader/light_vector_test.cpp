#include "altadore/shader/light_vector.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(LightVectorTest) {
};

TEST(LightVectorTest, Invoke) {
  std::vector<scoped_refptr<const Variant> > args;

  scoped_refptr<const Variant> var;
  scoped_refptr<Invokable> object;

  object.Reset(new Light(new Point3(1.0, 2.0, 3.0), new Color(0.1, 0.2, 0.3)));
  var.Reset(new Variant(object.ptr()));
  args.push_back(var.ptr());

  LightVector lights;
  EXPECT_EQ(lights.Invoke("AddLight", args, var.Receive()), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.ptr());
}

TEST(LightVectorTest, InvokeError) {
  std::vector<scoped_refptr<const Variant> > args;

  scoped_refptr<const Variant> var;

  LightVector lights;
  EXPECT_EQ(lights.Invoke("blah", args, var.Receive()),
            Invokable::RESULT_ERR_NAME);

  EXPECT_EQ(lights.Invoke("AddLight", args, var.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  var.Reset(new Variant(2.0));
  args.push_back(var.ptr());

  EXPECT_EQ(lights.Invoke("AddLight", args, var.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(LightVectorTest, Constructor) {
  LightVector lights;
  EXPECT_EQ(lights.lights().size(), 0);
}

TEST(LightVectorTest, AddLight) {
  LightVector lights;
  lights.AddLight(new Light(new Point3(), new Color()));
  EXPECT_EQ(lights.lights().size(), 1);
}

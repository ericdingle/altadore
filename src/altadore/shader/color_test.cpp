#include "altadore/shader/color.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(ColorTest) {
};

TEST(ColorTest, Create) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_refptr<Invokable> object;
  EXPECT_EQ(Color::Create(args, object.Receive()), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.ptr());

  memory::scoped_refptr<const Variant> var(new Variant(1.0));
  args.push_back(var.ptr());
  args.push_back(var.ptr());
  args.push_back(var.ptr());

  EXPECT_EQ(Color::Create(args, object.Receive()), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.ptr());
}

TEST(ColorTest, CreateError) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_refptr<const Variant> var(new Variant(1.0));
  args.push_back(var.ptr());
  args.push_back(var.ptr());

  memory::scoped_refptr<Invokable> object;
  EXPECT_EQ(Color::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  var.Reset(new Variant(2));
  args.push_back(var.ptr());

  EXPECT_EQ(Color::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(ColorTest, Constructor) {
  Color color1;
  EXPECT_EQ(color1.r(), 0);
  EXPECT_EQ(color1.g(), 0);
  EXPECT_EQ(color1.b(), 0);

  Color color2(0.5, 1.0, 1.5);
  EXPECT_EQ(color2.r(), 0.5);
  EXPECT_EQ(color2.g(), 1.0);
  EXPECT_EQ(color2.b(), 1.0);
}

TEST(ColorTest, CopyConstructor) {
  Color color1(0.5, 1.0, 1.5);
  Color color2(color1);
  EXPECT_TRUE(color1 == color2);
}

TEST(ColorTest, DoubleMultiplication) {
  Color color = Color(0.25, 0.5, 0.75) * 2;
  EXPECT_TRUE(color == Color(0.5, 1.0, 1.0));
}

TEST(ColorTest, DoubleAddition) {
  Color color(0.25, 0.5, 0.75);
  color += 0.25;
  EXPECT_TRUE(color == Color(0.5, 0.75, 1.0));
}

TEST(ColorTest, ColorAddition) {
  Color color(0.25, 0.5, 0.75);
  color += color;
  EXPECT_TRUE(color == Color(0.5, 1.0, 1.0));
}

TEST(ColorTest, ColorMultiplication) {
  Color color(0.25, 0.5, 0.75);
  color *= color;
  EXPECT_TRUE(color == Color(0.0625, 0.25, 0.5625));
}

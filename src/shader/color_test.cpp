#include "shader/color.h"

#include "third_party/chaparral/src/executer/variant.h"

TEST_CASE(ColorTest) {
};

TEST(ColorTest, Create) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Invokable::RESULT_OK, Color::Create(args, &object));
  EXPECT_NOT_NULL(object.get());

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(Invokable::RESULT_OK, Color::Create(args, &object));
  EXPECT_NOT_NULL(object.get());
}

TEST(ColorTest, CreateError) {
  std::vector<std::shared_ptr<const Variant>> args;

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE, Color::Create(args, &object));

  var.reset(new Variant(2));
  args.push_back(var);

  EXPECT_EQ(Invokable::RESULT_ERR_ARG_TYPE, Color::Create(args, &object));
}

TEST(ColorTest, Constructor) {
  Color color1;
  EXPECT_EQ(0, color1.r());
  EXPECT_EQ(0, color1.g());
  EXPECT_EQ(0, color1.b());

  Color color2(0.5, 1.0, 1.5);
  EXPECT_EQ(0.5, color2.r());
  EXPECT_EQ(1, color2.g());
  EXPECT_EQ(1, color2.b());
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

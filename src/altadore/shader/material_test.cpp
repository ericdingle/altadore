#include "altadore/shader/material.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(MaterialTest) {
};

TEST(MaterialTest, Create) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var;
  std::shared_ptr<Invokable> object;

  object.reset(new Color(0.1, 0.2, 0.3));
  var.reset(new Variant(object));
  args.push_back(var);

  var.reset(new Variant(25.0));
  args.push_back(var);

  var.reset(new Variant(0.4));
  args.push_back(var);

  EXPECT_EQ(Material::Create(args, &object), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());
}

TEST(MaterialTest, CreateError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Material::Create(args, &object),
            Invokable::RESULT_ERR_ARG_SIZE);

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(Material::Create(args, &object),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(MaterialTest, Constructor) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
}

TEST(MaterialTest, Color) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  const Color* color = mat.color();
  EXPECT_EQ(color->r(), 0.1);
  EXPECT_EQ(color->g(), 0.2);
  EXPECT_EQ(color->b(), 0.3);
}

TEST(MaterialTest, Shininess) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  double shininess = mat.shininess();
  EXPECT_EQ(shininess, 25);
}

TEST(MaterialTest, Reflectivity) {
  Material mat(std::make_shared<Color>(0.1, 0.2, 0.3), 25.0, 0.4);
  double reflectivity = mat.reflectivity();
  EXPECT_EQ(reflectivity, 0.4);
}

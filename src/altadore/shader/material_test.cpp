#include "altadore/shader/material.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(MaterialTest) {
};

TEST(MaterialTest, Create) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_refptr<const Variant> var;

  memory::scoped_refptr<Invokable> color(new Color(0.1, 0.2, 0.3));
  var.Reset(new Variant(color.ptr()));
  args.push_back(var.ptr());

  var.Reset(new Variant(25.0));
  args.push_back(var.ptr());

  var.Reset(new Variant(0.4));
  args.push_back(var.ptr());

  memory::scoped_ptr<Invokable> object;
  EXPECT_EQ(Material::Create(args, object.Receive()), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.ptr());
}

TEST(MaterialTest, CreateError) {
  std::vector<memory::scoped_refptr<const Variant> > args;

  memory::scoped_ptr<Invokable> object;
  EXPECT_EQ(Material::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_SIZE);

  memory::scoped_refptr<const Variant> var(new Variant(1.0));
  args.push_back(var.ptr());
  args.push_back(var.ptr());
  args.push_back(var.ptr());

  EXPECT_EQ(Material::Create(args, object.Receive()),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(MaterialTest, Constructor) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
}

TEST(MaterialTest, Color) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  const Color* color = mat.color();
  EXPECT_EQ(color->r(), 0.1);
  EXPECT_EQ(color->g(), 0.2);
  EXPECT_EQ(color->b(), 0.3);
}

TEST(MaterialTest, Shininess) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  double shininess = mat.shininess();
  EXPECT_EQ(shininess, 25);
}

TEST(MaterialTest, Reflectivity) {
  Material mat(new Color(0.1, 0.2, 0.3), 25.0, 0.4);
  double reflectivity = mat.reflectivity();
  EXPECT_EQ(reflectivity, 0.4);
}

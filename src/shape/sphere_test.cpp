#include "altadore/shape/sphere.h"

#include "altadore/algebra/ray.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(SphereTest) {
};

TEST(SphereTest, Create) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Invokable::RESULT_OK, Sphere::Create(args, &object));
  EXPECT_NOT_NULL(object.get());
}

TEST(SphereTest, CreateError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Invokable::RESULT_ERR_ARG_SIZE, Sphere::Create(args, &object));
}

TEST(SphereTest, FindIntersection) {
  Sphere sphere;
  double t;
  Point3 point;
  Vector3 normal;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.FindIntersection(ray1, &t, &point, &normal));
  EXPECT_EQ(1, t);
  EXPECT_EQ(0, point[0]);
  EXPECT_EQ(0, point[1]);
  EXPECT_EQ(1, point[2]);
  EXPECT_EQ(0, normal[0]);
  EXPECT_EQ(0, normal[1]);
  EXPECT_EQ(1, normal[2]);

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.FindIntersection(ray2, &t, &point, &normal));
}

TEST(SphereTest, HasIntersection) {
  Sphere sphere;

  Ray ray1(Point3(0, 0, 2), Vector3(0, 0, -1));
  EXPECT_TRUE(sphere.HasIntersection(ray1, 2));
  EXPECT_FALSE(sphere.HasIntersection(ray1, 0.5));

  Ray ray2(Point3(0, 0, 2), Vector3(0, 0, 1));
  EXPECT_FALSE(sphere.HasIntersection(ray2, 20));
}

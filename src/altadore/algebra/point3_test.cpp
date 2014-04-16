#include "altadore/algebra/point3.h"

#include "altadore/algebra/vector3.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/executer/variant.h"

TEST_CASE(Point3Test) {
};

TEST(Point3Test, Create) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Point3::Create(args, &object), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);
  args.push_back(var);

  EXPECT_EQ(Point3::Create(args, &object), Invokable::RESULT_OK);
  EXPECT_NOT_NULL(object.get());
}

TEST(Point3Test, CreateError) {
  std::vector<std::shared_ptr<const Variant> > args;

  std::shared_ptr<const Variant> var(new Variant(1.0));
  args.push_back(var);
  args.push_back(var);

  std::shared_ptr<Invokable> object;
  EXPECT_EQ(Point3::Create(args, &object),
            Invokable::RESULT_ERR_ARG_SIZE);

  var.reset(new Variant(2));
  args.push_back(var);

  EXPECT_EQ(Point3::Create(args, &object),
            Invokable::RESULT_ERR_ARG_TYPE);
}

TEST(Point3Test, Constructor) {
  Point3 point1;
  EXPECT_EQ(point1[0], 0);
  EXPECT_EQ(point1[1], 0);
  EXPECT_EQ(point1[2], 0);

  Point3 point2(1.0, 2.0, 3.0);
  EXPECT_EQ(point2[0], 1.0);
  EXPECT_EQ(point2[1], 2.0);
  EXPECT_EQ(point2[2], 3.0);
}

TEST(Point3Test, CopyConstructor) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2(point1);
  EXPECT_TRUE(point1 == point2);
}

TEST(Point3Test, AssignmentOperator) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2;
  point2 = point1;
  EXPECT_TRUE(point1 == point2);
}

TEST(Point3Test, IndexOperator) {
  Point3 point(1.0, 2.0, 3.0);
  point[0] = 4.0;
  point[1] = 5.0;
  point[2] = 6.0;
  EXPECT_TRUE(point == Point3(4.0, 5.0, 6.0));
}

TEST(Point3Test, VectorAddition) {
  Point3 point1(1.0, 2.0, 3.0);
  Vector3 vector(4.0, 5.0, 6.0);
  Point3 point2(point1 + vector);
  EXPECT_TRUE(point2 == Point3(5.0, 7.0, 9.0));
}

TEST(Point3Test, PointSubtraction) {
  Point3 point1(1.0, 2.0, 3.0);
  Point3 point2(6.0, 5.0, 4.0);
  Vector3 vector(point2 - point1);
  EXPECT_TRUE(vector == Vector3(5.0, 3.0, 1.0));
}

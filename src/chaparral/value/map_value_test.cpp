#include "chaparral/value/map_value.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/value/number_value.h"

TEST_CASE(MapValueTest) {
};

TEST(MapValueTest, GetAsMap) {
  MapValue map;
  MapValue* map_ptr;
  EXPECT_TRUE(map.GetAsMap(&map_ptr));
  EXPECT_EQ(&map, map_ptr);
}

TEST(MapValueTest, GetAndSet) {
  MapValue map;

  memory::scoped_refptr<Value> value;
  EXPECT_FALSE(map.Get("num", value.Receive()));

  {
    memory::scoped_refptr<Value> num_val = new NumberValue(5);
    map.Set("num", num_val.ptr());
  }

  EXPECT_TRUE(map.Get("num", value.Receive()));
  EXPECT_EQ(value->ref_count(), 2);

  double d;
  EXPECT_TRUE(value->GetAsNumber(&d));
  EXPECT_EQ(d, 5);
}

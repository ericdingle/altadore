#include "chaparral/value/map_value.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/value/number_value.h"

TEST_CASE(MapValueTest) {
};

TEST(MapValueTest, GetAndSet) {
  MapValue map;

  memory::scoped_ref<Value> value = map.Get("num");
  EXPECT_NULL(value.ptr());

  {
    memory::scoped_ref<Value> num_val = new NumberValue(5);
    map.Set("num", num_val.ptr());
  }

  value = map.Get("num");
  EXPECT_NOT_NULL(value.ptr());
  EXPECT_EQ(value->ref_count(), 2);

  NumberValue* num_val = dynamic_cast<NumberValue*>(value.ptr());
  EXPECT_NOT_NULL(num_val);
  EXPECT_EQ(num_val->Get(), 5);
}

TEST(MapValueTest, ToString) {
  MapValue map;
  EXPECT_EQ(map.ToString(), "Map");
}

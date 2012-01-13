#ifndef CHAPARRAL_EXECUTER_VARIANT_H_
#define CHAPARRAL_EXECUTER_VARIANT_H_

#include "bonavista/logging/assert.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class Variant {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  ~Variant();

  template <typename T>
  bool Get(T* out) const {
    ASSERT(out);

    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.ptr());
    if (data) {
      *out = data->value;
      return true;
    }

    return false;
  }

  template <typename T>
  bool Get(T** out) const {
    ASSERT(out);

    const Data<memory::scoped_refptr<T> >* ref_data =
        dynamic_cast<const Data<memory::scoped_refptr<T> >*>(data_.ptr());
    if (ref_data) {
      ref_data->value->AddRef();
      *out = ref_data->value.ptr();
      return true;
    }

    return false;
  }

 private:
  struct DataBase {
    virtual ~DataBase();
  };

  template <typename T>
  struct Data : public DataBase {
    Data(T t) : value(t) {}
    virtual ~Data() {}

    T value;
  };

  memory::scoped_ptr<const DataBase> data_;

  DISALLOW_COPY_AND_ASSIGN(Variant);
};

#endif

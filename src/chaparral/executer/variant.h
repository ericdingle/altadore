#ifndef CHAPARRAL_EXECUTER_VARIANT_H_
#define CHAPARRAL_EXECUTER_VARIANT_H_

#include "bonavista/logging/assert.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/util/macros.h"

class Variant : public memory::RefCount {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  template <typename T>
  explicit Variant(T* value) : data_(new ScopedData<T>(value)) {}
  ~Variant();

  template <typename T>
  bool Get(T* out) const {
    ASSERT(out);

    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.ptr());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

  template <typename T>
  bool Get(T** out) const {
    ASSERT(out);

    const ScopedData<T>* data = dynamic_cast<const ScopedData<T>*>(data_.ptr());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

 private:
  class DataBase {
   public:
    DataBase();
    virtual ~DataBase();

   private:
    DISALLOW_COPY_AND_ASSIGN(DataBase);
  };

  template <typename T>
  class Data : public DataBase {
   public:
    explicit Data(T t) : value_(t) {}
    virtual ~Data() {}

    const T& value() const { return value_; }

   private:
    T value_;

    DISALLOW_COPY_AND_ASSIGN(Data);
  };

  template <typename T>
  class ScopedData : public DataBase {
   public:
    explicit ScopedData(T* t) : value_(t) {}
    virtual ~ScopedData() {}

    T* value() const { return value_.ptr(); }

   private:
    memory::scoped_ptr<T> value_;

    DISALLOW_COPY_AND_ASSIGN(ScopedData);
  };

  memory::scoped_ptr<const DataBase> data_;

  DISALLOW_COPY_AND_ASSIGN(Variant);
};

#endif

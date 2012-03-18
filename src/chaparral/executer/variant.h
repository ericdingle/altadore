#ifndef CHAPARRAL_EXECUTER_VARIANT_H_
#define CHAPARRAL_EXECUTER_VARIANT_H_

#include "bonavista/base/macros.h"
#include "bonavista/memory/ref_counted.h"
#include "bonavista/memory/scoped_refptr.h"

class Variant : public RefCounted {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  template <typename T>
  explicit Variant(T* value) : data_(new ScopedRefData<T>(value)) {}
  ~Variant();

  template <typename T>
  bool Get(T* out) const {
    DCHECK(out);

    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.ptr());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

  template <typename T>
  bool Get(T** out) const {
    DCHECK(out);

    const ScopedRefData<T>* data = dynamic_cast<const ScopedRefData<T>*>(data_.ptr());
    if (data) {
      data->value()->AddRef();
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
  class ScopedRefData : public DataBase {
   public:
    explicit ScopedRefData(T* t) : value_(t) {}
    virtual ~ScopedRefData() {}

    T* value() const { return value_.ptr(); }

   private:
    scoped_refptr<T> value_;

    DISALLOW_COPY_AND_ASSIGN(ScopedRefData);
  };

  scoped_ptr<const DataBase> data_;

  DISALLOW_COPY_AND_ASSIGN(Variant);
};

#endif

#ifndef ALTADORE_SHADER_LIGHT_VECTOR_H_
#define ALTADORE_SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "altadore/shader/light.h"
#include "altadore/util/invokable.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class LightVector : public Invokable {
 public:
  LightVector();
  virtual ~LightVector();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      const Variant** var);

  void AddLight(const Light* light);

  const std::vector<memory::scoped_refptr<const Light> >& lights() const { return lights_; }

 private:
  std::vector<memory::scoped_refptr<const Light> > lights_;

  DISALLOW_COPY_AND_ASSIGN(LightVector);
};

#endif

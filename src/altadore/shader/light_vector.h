#ifndef ALTADORE_SHADER_LIGHT_VECTOR_H_
#define ALTADORE_SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "altadore/shader/light.h"
#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_refptr.h"
#include "chaparral/executer/invokable.h"

class LightVector : public Invokable {
 public:
  LightVector();
  virtual ~LightVector();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<scoped_refptr<const Variant> >& args,
      const Variant** var);

  void AddLight(const Light* light);

  const std::vector<scoped_refptr<const Light> >& lights() const { return lights_; }

 private:
  std::vector<scoped_refptr<const Light> > lights_;

  DISALLOW_COPY_AND_ASSIGN(LightVector);
};

#endif

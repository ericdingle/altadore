#ifndef ALTADORE_SHADER_LIGHT_VECTOR_H_
#define ALTADORE_SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "altadore/shader/light.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"

class LightVector : public memory::RefCount {
 public:
  LightVector();
  ~LightVector();

  void AddLight(const Light* light);

  const std::vector<memory::scoped_refptr<const Light> >& lights() const { return lights_; }

 private:
  std::vector<memory::scoped_refptr<const Light> > lights_;

  DISALLOW_COPY_AND_ASSIGN(LightVector);
};

#endif

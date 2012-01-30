#ifndef ALTADORE_VISUAL_LIGHT_VECTOR_H_
#define ALTADORE_VISUAL_LIGHT_VECTOR_H_

#include <vector>
#include "altadore/visual/light.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"

class LightVector : public memory::RefCount {
public:
  LightVector();
  ~LightVector();

  void AddLight(const Light* light);

  const std::vector<memory::scoped_refptr<const Light> >& lights() const { return lights_; }

private:
  LightVector(const LightVector&);
  void operator=(const LightVector&);

  std::vector<memory::scoped_refptr<const Light> > lights_;
};

#endif

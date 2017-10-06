#ifndef ALTADORE_SHADER_LIGHT_VECTOR_H_
#define ALTADORE_SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "altadore/shader/light.h"
#include "bonavista/base/macros.h"
#include "chaparral/executer/invokable.h"

class LightVector : public Invokable {
 public:
  LightVector();
  virtual ~LightVector();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var);

  void AddLight(const std::shared_ptr<const Light>& light);

  const std::vector<std::shared_ptr<const Light> >& lights() const { return lights_; }

 private:
  std::vector<std::shared_ptr<const Light> > lights_;

  DISALLOW_COPY_AND_ASSIGN(LightVector);
};

#endif

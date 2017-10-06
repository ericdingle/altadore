#ifndef SHADER_LIGHT_VECTOR_H_
#define SHADER_LIGHT_VECTOR_H_

#include <vector>
#include "shader/light.h"
#include "third_party/chaparral/src/executer/invokable.h"

class LightVector : public Invokable {
 public:
  LightVector();
  LightVector(const LightVector&) = delete;
  LightVector& operator=(const LightVector&) = delete;
  virtual ~LightVector();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant>>& args,
      std::shared_ptr<const Variant>* var);

  void AddLight(const std::shared_ptr<const Light>& light);

  const std::vector<std::shared_ptr<const Light>>& lights() const { return lights_; }

 private:
  std::vector<std::shared_ptr<const Light>> lights_;
};

#endif  // SHADER_LIGHT_VECTOR_H_

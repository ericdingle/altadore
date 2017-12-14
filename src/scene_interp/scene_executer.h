#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <map>
#include <memory>
#include <string>
#include "third_party/chaparral/src/executer/any.h"
#include "third_party/chaparral/src/executer/executer.h"

using SceneFunc = std::function<StatusOr<std::shared_ptr<Any>>(
    const std::vector<const Node*>&)>;

class SceneExecuter : public Executer {
 public:
  explicit SceneExecuter(Parser* parser);
  SceneExecuter(const SceneExecuter&) = delete;
  SceneExecuter& operator=(const SceneExecuter&) = delete;
  ~SceneExecuter() override = default;

  std::shared_ptr<Any> GetVariable(const std::string& name) const;
  void SetVariable(const std::string& name,
                   const std::shared_ptr<Any>& any);

 protected:
  StatusOr<std::shared_ptr<Any>> ExecuteNode(const Node* node) override;

 private:
  StatusOr<std::shared_ptr<Any>> ExecuteAssignment(const Node* node);
  StatusOr<std::shared_ptr<Any>> ExecuteDotAccessor(const Node* node);
  StatusOr<std::shared_ptr<Any>> ExecuteFunction(const Node* node);
  StatusOr<std::shared_ptr<Any>> ExecuteIdentifier(const Node* node);
  StatusOr<std::shared_ptr<Any>> ExecuteNumber(const Node* node);

  std::map<std::string, std::shared_ptr<Any>> variables_;
};

#endif  // SCENE_INTERP_EXECUTER_H_

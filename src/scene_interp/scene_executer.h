#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <map>
#include <string>
#include "third_party/chaparral/src/executer/any.h"
#include "third_party/chaparral/src/executer/executer.h"

using SceneFunc = std::function<StatusOr<Any>(
    const std::vector<const Node*>&, int line, int column)>;

class SceneExecuter : public Executer {
 public:
  explicit SceneExecuter(Parser* parser);
  SceneExecuter(const SceneExecuter&) = delete;
  SceneExecuter& operator=(const SceneExecuter&) = delete;
  ~SceneExecuter() override = default;

  Any GetVariable(const std::string& name) const;
  void SetVariable(const std::string& name, const Any& any);

 protected:
  StatusOr<Any> ExecuteNode(const Node* node) override;

  StatusOr<Any> ExecuteAssignment(const Node* node);
  StatusOr<Any> ExecuteDotAccessor(const Node* node);
  StatusOr<Any> ExecuteFunction(const Node* node);
  StatusOr<Any> ExecuteIdentifier(const Node* node);
  StatusOr<Any> ExecuteNumber(const Node* node);

  static Status ExpectSize(const std::vector<const Node*>& args, int size,
                           int line, int column);

  StatusOr<Any> CreatePoint3(
      const std::vector<const Node*>&, int line, int column);

  std::map<std::string, Any> variables_;
};

#endif  // SCENE_INTERP_EXECUTER_H_

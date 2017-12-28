#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <map>
#include <string>
#include "scene_interp/scene_object.h"
#include "third_party/chaparral/src/executer/any.h"
#include "third_party/chaparral/src/executer/executer.h"

using SceneFunc = std::function<StatusOr<Any>(
    const Token&, const std::vector<const Node*>&)>;

class SceneExecuter : public Executer {
 public:
  explicit SceneExecuter(Parser* parser);
  SceneExecuter(const SceneExecuter&) = delete;
  SceneExecuter& operator=(const SceneExecuter&) = delete;
  ~SceneExecuter() override = default;

  Any GetVariable(const std::string& name) const;
  void SetVariable(const std::string& name, const Any& any);

  static Status ExpectSize(
      const Token& token, const std::vector<const Node*>& args, int size);

 protected:
  friend class SceneObject;

  StatusOr<Any> ExecuteNode(const Node* node) override;

  StatusOr<Any> ExecuteAssignment(const Node* node);
  StatusOr<Any> ExecuteDotAccessor(const Node* node);
  StatusOr<Any> ExecuteFunction(const Node* node);
  StatusOr<Any> ExecuteIdentifier(const Node* node);
  StatusOr<Any> ExecuteNumber(const Node* node);

  StatusOr<Any> CreateColor(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateCube(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateLight(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateMaterial(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreatePoint3(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateShapeNode(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateSphere(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<Any> CreateTransformNode(
      const Token& token, const std::vector<const Node*>& args);

  std::map<std::string, Any> variables_;
};

#endif  // SCENE_INTERP_EXECUTER_H_

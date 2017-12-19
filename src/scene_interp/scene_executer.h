#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <map>
#include <string>
#include "scene_interp/scene_object.h"
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
  template <typename T>
  StatusOr<std::shared_ptr<T>> ExecuteNodeObject(const Node* node);

  StatusOr<Any> ExecuteAssignment(const Node* node);
  StatusOr<Any> ExecuteDotAccessor(const Node* node);
  StatusOr<Any> ExecuteFunction(const Node* node);
  StatusOr<Any> ExecuteIdentifier(const Node* node);
  StatusOr<Any> ExecuteNumber(const Node* node);

  static Status ExpectSize(const std::vector<const Node*>& args, int size,
                           int line, int column);

  StatusOr<Any> CreateColor(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateCube(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateLight(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateMaterial(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreatePoint3(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateShapeNode(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateSphere(
      const std::vector<const Node*>&, int line, int column);
  StatusOr<Any> CreateTransformNode(
      const std::vector<const Node*>&, int line, int column);

  std::map<std::string, Any> variables_;
};

template <typename T>
StatusOr<std::shared_ptr<T>> SceneExecuter::ExecuteNodeObject(const Node* node) {
  ASSIGN_OR_RETURN(auto obj, ExecuteNodeT<std::shared_ptr<SceneObject>>(node));
  auto ptr = std::dynamic_pointer_cast<T>(obj);
  if (!ptr) {
    return Status(std::string("Expected type: ") + typeid(T).name(),
                  node->token().line(), node->token().column());
  }
  return ptr;
}

#endif  // SCENE_INTERP_EXECUTER_H_

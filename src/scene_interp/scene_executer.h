#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <functional>
#include <map>
#include <string>
#include "scene_interp/scene_object.h"
#include "third_party/chaparral/src/executer/executer.h"

using SceneFunc = std::function<StatusOr<std::any>(
    const Token&, const std::vector<const Node*>&)>;

class SceneExecuter : public Executer {
 public:
  explicit SceneExecuter(Parser* parser);
  SceneExecuter(const SceneExecuter&) = delete;
  SceneExecuter& operator=(const SceneExecuter&) = delete;
  ~SceneExecuter() override = default;

  std::any GetVariable(std::string_view name) const;
  void SetVariable(std::string_view name, const std::any& any);

  static Status ExpectSize(
      const Token& token, const std::vector<const Node*>& args, int size);

 protected:
  friend class SceneObject;

  StatusOr<std::any> ExecuteNode(const Node& node) override;

  StatusOr<std::any> ExecuteAssignment(const Node& node);
  StatusOr<std::any> ExecuteDotAccessor(const Node& node);
  StatusOr<std::any> ExecuteFunction(const Node& node);
  StatusOr<std::any> ExecuteIdentifier(const Node& node);
  StatusOr<std::any> ExecuteNumber(const Node& node);

  StatusOr<std::any> CreateColor(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateCube(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateLight(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateMaterial(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreatePoint3(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateShapeNode(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateSphere(
      const Token& token, const std::vector<const Node*>& args);
  StatusOr<std::any> CreateTransformNode(
      const Token& token, const std::vector<const Node*>& args);

  std::unordered_map<std::string_view, std::any> variables_;
};

#endif  // SCENE_INTERP_EXECUTER_H_

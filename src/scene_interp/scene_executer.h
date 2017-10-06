#ifndef SCENE_INTERP_EXECUTER_H_
#define SCENE_INTERP_EXECUTER_H_

#include <map>
#include <memory>
#include <string>
#include "third_party/chaparral/src/executer/executer.h"
#include "third_party/chaparral/src/executer/variant.h"

class SceneExecuter : public Executer {
 public:
  explicit SceneExecuter(Parser* parser);
  SceneExecuter(const SceneExecuter&) = delete;
  SceneExecuter& operator=(const SceneExecuter&) = delete;
  virtual ~SceneExecuter();

  void SetVar(const std::string& name,
              const std::shared_ptr<const Variant>& var);

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              std::shared_ptr<const Variant>* var);

 private:
  bool ExecuteDotAccessor(const ASTNode* node,
                          std::shared_ptr<const Variant>* var);
  bool ExecuteAssignment(const ASTNode* node,
                         std::shared_ptr<const Variant>* var);
  bool ExecuteIdentifier(const ASTNode* node,
                         std::shared_ptr<const Variant>* var);
  bool ExecuteNew(const ASTNode* node, std::shared_ptr<const Variant>* var);
  bool ExecuteNumber(const ASTNode* node, std::shared_ptr<const Variant>* var);

  std::map<std::string, std::shared_ptr<const Variant>> var_map_;
};

#endif  // SCENE_INTERP_EXECUTER_H_

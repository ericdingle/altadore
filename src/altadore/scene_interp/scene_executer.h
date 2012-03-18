#ifndef ALTADORE_SCENE_INTERP_EXECUTER_H_
#define ALTADORE_SCENE_INTERP_EXECUTER_H_

#include <map>
#include <string>
#include "bonavista/base/macros.h"
#include "bonavista/memory/scoped_refptr.h"
#include "chaparral/executer/executer.h"
#include "chaparral/executer/variant.h"

class SceneExecuter : public Executer {
 public:
  SceneExecuter(Parser* parser);
  ~SceneExecuter();

  void SetVar(const std::string& name, const Variant* var);

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node, const Variant** var);

 private:
  bool ExecuteDotAccessor(const ASTNode* node, const Variant** var);
  bool ExecuteAssignment(const ASTNode* node, const Variant** var);
  bool ExecuteIdentifier(const ASTNode* node, const Variant** var);
  bool ExecuteNew(const ASTNode* node, const Variant** var);
  bool ExecuteNumber(const ASTNode* node, const Variant** var);

  std::map<std::string, scoped_refptr<const Variant> > var_map_;

  DISALLOW_COPY_AND_ASSIGN(SceneExecuter);
};

#endif

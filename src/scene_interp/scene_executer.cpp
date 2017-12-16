#include "scene_interp/scene_executer.h"

#include <algorithm>
#include <assert.h>
#include "algebra/matrix4.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_object.h"

SceneExecuter::SceneExecuter(Parser* parser) : Executer(parser) {
  SetVariable("AXIS_X", Any(static_cast<double>(Matrix4::AXIS_X)));
  SetVariable("AXIS_Y", Any(static_cast<double>(Matrix4::AXIS_Y)));
  SetVariable("AXIS_Z", Any(static_cast<double>(Matrix4::AXIS_Z)));
}

Any SceneExecuter::GetVariable(const std::string& name) const {
  auto it = variables_.find(name);
  return it == variables_.end() ? Any() : it->second;
}

void SceneExecuter::SetVariable(const std::string& name, const Any& any) {
  variables_[name] = any;
}

StatusOr<Any> SceneExecuter::ExecuteNode(const Node* node) {
  switch (node->token().type()) {
    case SceneLexer::TYPE_EQUAL:
      return ExecuteAssignment(node);
    case SceneLexer::TYPE_DOT:
      return ExecuteDotAccessor(node);
    case SceneLexer::TYPE_LEFT_PARENTHESIS:
      return ExecuteFunction(node);
    case SceneLexer::TYPE_IDENTIFIER:
      return ExecuteIdentifier(node);
    case SceneLexer::TYPE_NUMBER:
      return ExecuteNumber(node);
    default:
      assert(false);
      return Status("WTF!", 1, 1);
  }
}

StatusOr<Any> SceneExecuter::ExecuteAssignment(const Node* node) {
  const std::string& name = node->children()[0]->token().value();
  ASSIGN_OR_RETURN(auto value, ExecuteNode(node->children()[1].get()));
  variables_[name] = value;
  return value;
}

StatusOr<Any> SceneExecuter::ExecuteDotAccessor(const Node* node) {
  ASSIGN_OR_RETURN(auto obj, ExecuteNodeT<std::shared_ptr<SceneObject>>(node->children()[0].get()));
  return obj->Get(node->children()[1]->token().value());
}

StatusOr<Any> SceneExecuter::ExecuteFunction(const Node* node) {
  const auto& children = node->children();
  ASSIGN_OR_RETURN(auto func, ExecuteNodeT<SceneFunc>(children[0].get()));

  std::vector<const Node*> args;
  std::transform(children.begin() + 1, children.end(), std::back_inserter(args),
                 [](const std::unique_ptr<const Node>& n) { return n.get(); });

  return func(args);
}

StatusOr<Any> SceneExecuter::ExecuteIdentifier(const Node* node) {
  const std::string& name = node->token().value();

  if (variables_.count(name) == 0) {
    return Status(name + " is undefined", node->token().line(),
                  node->token().column());
  }

  return variables_[name];
}

StatusOr<Any> SceneExecuter::ExecuteNumber(const Node* node) {
  double value = atof(node->token().value().c_str());
  return Any(value);
}

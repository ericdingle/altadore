#include "chaparral/executer/executer.h"

#include "bonavista/logging/assert.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/executer/executer.h"
#include "chaparral/executer/variant.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"
#include "chaparral/parser/parser.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const {
    ASSERT(type);
    ASSERT(value);
    ASSERT(count);
    ASSERT(error);

    if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    *type = TYPE_DIGIT;
    *value = input[index];
    *count = 1;
    return true;
  }
};

class TestParser : public Parser {
 public:
  TestParser(TokenStream* stream) : Parser(stream) {}

 protected:
  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root) {
    ASSERT(root);
    *root = new ASTNode(token);
    return true;
  }
};

class TestExecuter : public Executer {
 public:
  TestExecuter(Parser* parser) : Executer(parser) {}

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node, const Variant** var) {
    ASSERT(node);
    ASSERT(var);

    uint digit = node->token()->value()[0] - 0x30;
    if (digit == 9) {
      position_ = node->token()->position();
      error_ = "No nines!";
      return false;
    }

    *var = new Variant(digit);
    return true;
  }
};

TEST_CASE(ExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new TestParser(stream_.ptr()));
    executer_.Reset(new TestExecuter(parser_.ptr()));
  }

  TestLexer lexer_;
  memory::scoped_ptr<TokenStream> stream_;
  memory::scoped_ptr<Parser> parser_;
  memory::scoped_ptr<Executer> executer_;
  memory::scoped_ptr<const Variant> var_;
};

TEST(ExecuterTest, Empty) {
  Init("");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_NULL(var_.ptr());
}

TEST(ExecuterTest, BadToken) {
  Init("a");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(ExecuterTest, Execute) {
  Init("5");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  uint i = 0;
  EXPECT_TRUE(var_->Get(&i));
  EXPECT_EQ(i, 5);
}

TEST(ExecuterTest, ExecuteError) {
  Init("9");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

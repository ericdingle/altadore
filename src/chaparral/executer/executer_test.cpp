#include "chaparral/executer/executer.h"

#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/memory/scoped_refptr.h"
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
    DCHECK(type);
    DCHECK(value);
    DCHECK(count);
    DCHECK(error);

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
    DCHECK(root);
    *root = new ASTNode(token);
    return true;
  }
};

class TestExecuter : public Executer {
 public:
  TestExecuter(Parser* parser) : Executer(parser) {}

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node, const Variant** var) {
    DCHECK(node);
    DCHECK(var);

    uint digit = node->token()->value()[0] - 0x30;
    if (digit == 9) {
      position_ = node->token()->position();
      error_ = "No nines!";
      return false;
    }

    scoped_refptr<const Variant> var_ref(new Variant(digit));
    *var = var_ref.Release();
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
  scoped_ptr<TokenStream> stream_;
  scoped_ptr<Parser> parser_;
  scoped_ptr<Executer> executer_;
  scoped_refptr<const Variant> var_;
};

TEST(ExecuterTest, Empty) {
  Init("");
  EXPECT_FALSE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_NULL(var_.ptr());
  EXPECT_FALSE(executer_->HasInput());
}

TEST(ExecuterTest, BadToken) {
  Init("a");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
  EXPECT_TRUE(executer_->HasInput());
}

TEST(ExecuterTest, Execute) {
  Init("5");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  uint i = 0;
  EXPECT_TRUE(var_->Get(&i));
  EXPECT_EQ(i, 5);
  EXPECT_FALSE(executer_->HasInput());
}

TEST(ExecuterTest, ExecuteError) {
  Init("9");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(ExecuterTest, ExecuteT) {
  Init("5");
  uint i = 0;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(i, 5);
}

TEST(ExecuterTest, ExecuteTError) {
  uint i = 0;
  Init("");
  EXPECT_FALSE(executer_->ExecuteT(&i));
  EXPECT_FALSE(executer_->error().empty());

  Init("9");
  EXPECT_FALSE(executer_->ExecuteT(&i));
  EXPECT_FALSE(executer_->error().empty());

  double d = 0;
  Init("5");
  EXPECT_FALSE(executer_->ExecuteT(&d));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(ExecuterTest, ExecuteAll) {
  Init("1 2 3 4 5");
  EXPECT_TRUE(executer_->ExecuteAll());
  scoped_ptr<const Token> token;
  EXPECT_TRUE(stream_->GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
}

TEST(ExecuterTest, ExecuteAllError) {
  Init("1 3 5 7 9");
  EXPECT_FALSE(executer_->ExecuteAll());
}

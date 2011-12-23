#include "chaparral/parser/parser.h"

#include "bonavista/string/format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT,
    TYPE_PLUS
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const {
    const char& c = input[index];
    if (c == '+') {
      *type = TYPE_PLUS;
    } else if (IsDigit(c)) {
      *type = TYPE_DIGIT;
    } else {
      *error = string::Format("Unexpected token: %c", c);
      return false;
    }

    *value = c;
    *count = 1;
    return true;
  }
};

class TestParser : public Parser {
 public:
  virtual uint GetBindingPower(int type) const {
    if (type == TestLexer::TYPE_PLUS)
        return 10;

    return 0;
  }

  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root) {
    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      *root = new ASTNode(token);

      // Weird behavior only for the test.
      if (token->value() == "0")
        return ConsumeToken(TestLexer::TYPE_DIGIT);

      return true;
    }

    position_ = token->position();
    error_ = string::Format("Unexpected token: %s", token->value().c_str());
    return false;
  }

  virtual bool ParseInfixToken(const Token* token, const ASTNode* left,
                               const ASTNode** root) {
    if (token->IsType(TestLexer::TYPE_PLUS)) {
      memory::scoped_ptr<ASTNode> node(new ASTNode(token));
      node->AddChild(left);

      memory::scoped_ptr<const ASTNode> right;
      if (!ParseExpression(10, right.Receive()))
        return false;
      node->AddChild(right.Release());

      *root = node.Release();
      return true;
    }

    position_ = token->position();
    error_ = string::Format("Unexpected token: %s", token->value().c_str());
    return false;
  }
};

TEST_CASE(ParserTest) {
 public:
  virtual void TearDown() {
    for (uint i = 0; i < nodes_.size(); ++i)
      delete nodes_[i];
  }

 protected:
  TestLexer lexer_;
  TestParser parser_;
  std::vector<const ASTNode*> nodes_;
};

TEST(ParserTest, Empty) {
  TokenStream stream(&lexer_, "");
  EXPECT_TRUE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(nodes_.size(), 0);
}

TEST(ParserTest, BadToken) {
  TokenStream stream(&lexer_, "a");
  EXPECT_FALSE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(parser_.position().line, 1);
  EXPECT_EQ(parser_.position().column, 1);
  EXPECT_FALSE(parser_.error().empty());
}

TEST(ParserTest, Prefix) {
  TokenStream stream(&lexer_, "1");
  EXPECT_TRUE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(nodes_.size(), 1);

  const ASTNode* node = nodes_[0];
  EXPECT_TRUE(node->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(node->children().size(), 0);
}

TEST(ParserTest, PrefixError) {
  TokenStream stream(&lexer_, "+");
  EXPECT_FALSE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(parser_.position().line, 1);
  EXPECT_EQ(parser_.position().column, 1);
  EXPECT_FALSE(parser_.error().empty());
}

TEST(ParserTest, Infix) {
  TokenStream stream(&lexer_, "1+2");
  EXPECT_TRUE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(nodes_.size(), 1);

  const ASTNode* node = nodes_[0];
  EXPECT_TRUE(node->token()->IsType(TestLexer::TYPE_PLUS));
  EXPECT_EQ(node->children().size(), 2);

  const ASTNode* child = node->children()[0];
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(child->token()->value(), "1");
  EXPECT_EQ(child->children().size(), 0);

  child = node->children()[1];
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(child->token()->value(), "2");
  EXPECT_EQ(child->children().size(), 0);
}

TEST(ParserTest, InfixError) {
  TokenStream stream(&lexer_, "1+");
  EXPECT_FALSE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(parser_.position().line, 1);
  EXPECT_EQ(parser_.position().column, 3);
  EXPECT_FALSE(parser_.error().empty());
}

TEST(ParserTest, ConsumeToken) {
  TokenStream stream(&lexer_, "01");
  EXPECT_TRUE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(nodes_.size(), 1);

  const ASTNode* node = nodes_[0];
  EXPECT_TRUE(node->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(node->token()->value(), "0");
  EXPECT_EQ(node->children().size(), 0);
}

TEST(ParserTest, ConsumeTokenError) {
  TokenStream stream(&lexer_, "0");
  EXPECT_FALSE(parser_.Parse(&stream, &nodes_));
  EXPECT_EQ(parser_.position().line, 1);
  EXPECT_EQ(parser_.position().column, 2);
  EXPECT_FALSE(parser_.error().empty());
}

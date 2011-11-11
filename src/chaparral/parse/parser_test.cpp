#include "chaparral/parse/parser.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/parse/identifier_symbol.h"
#include "chaparral/parse/number_symbol.h"
#include "chaparral/parse/string_symbol.h"

TEST_CASE(ParserTest) {
};

TEST(ParserTest, ParseEmptyInput) {
  TokenStream token_stream("");
  Parser parser;
  SymbolRef symbol;
  EXPECT_FALSE(parser.Parse(&token_stream, &symbol));
}

TEST(ParserTest, ParseNumber) {
  TokenStream token_stream("1");
  Parser parser;
  SymbolRef symbol;
  EXPECT_TRUE(parser.Parse(&token_stream, &symbol));
  EXPECT_EQ(symbol->value(), "1");
  EXPECT_NOT_NULL(dynamic_cast<const NumberSymbol*>(symbol.ptr()));
}

TEST(ParserTest, ParseString) {
  TokenStream token_stream("'asdf'");
  Parser parser;
  SymbolRef symbol;
  EXPECT_TRUE(parser.Parse(&token_stream, &symbol));
  EXPECT_EQ(symbol->value(), "'asdf'");
  EXPECT_NOT_NULL(dynamic_cast<const StringSymbol*>(symbol.ptr()));
}

TEST(ParserTest, ParseIdentifier) {
  TokenStream token_stream("table");
  Parser parser;
  SymbolRef symbol;
  EXPECT_TRUE(parser.Parse(&token_stream, &symbol));
  EXPECT_EQ(symbol->value(), "table");
  EXPECT_NOT_NULL(dynamic_cast<const IdentifierSymbol*>(symbol.ptr()));
}

TEST(ParserTest, ParseUnknown) {
  TokenStream token_stream(";");
  Parser parser;
  SymbolRef symbol;
  EXPECT_FALSE(parser.Parse(&token_stream, &symbol));
}

class TestPrefixSymbol : public PrefixSymbol {
public:
  TestPrefixSymbol(const Token* token) : PrefixSymbol(token) {}

  bool Parse(Parser* parser, SymbolRef* root) {
    if (!parser->Expression(&right_))
      return false;
    *root = this;
    return true;
  }

  const Symbol* right() const { return right_.ptr(); }

private:
  SymbolRef right_;
};

TEST(ParserTest, ParsePrefixSymbol) {
  Parser parser;
  parser.AddSymbol("a", &PrefixSymbol::Create<TestPrefixSymbol>);

  {
    TokenStream token_stream("a");
    SymbolRef symbol;
    EXPECT_FALSE(parser.Parse(&token_stream, &symbol));
  }

  {
    TokenStream token_stream("a 5");
    SymbolRef symbol;
    EXPECT_TRUE(parser.Parse(&token_stream, &symbol));
    EXPECT_EQ(symbol->value(), "a");
    EXPECT_NOT_NULL(dynamic_cast<const TestPrefixSymbol*>(symbol.ptr()));

    const Symbol* right = dynamic_cast<const TestPrefixSymbol*>(symbol.ptr())->right();
    EXPECT_NOT_NULL(right);
    EXPECT_NOT_NULL(dynamic_cast<const NumberSymbol*>(right));
  }
}


class PlusSymbol : public InfixSymbol {
public:
  static const uint BINDING_POWER = 10;

  PlusSymbol(const Token* token) : InfixSymbol(token) {}

  bool Parse(Parser* parser, const Symbol* left, SymbolRef* root) {
    left_ = left;
    if (!parser->Expression(&right_, BINDING_POWER))
      return false;
    *root = this;
    return true;
  }

  const Symbol* left() const { return left_.ptr(); }
  const Symbol* right() const { return right_.ptr(); }

private:
  SymbolRef left_;
  SymbolRef right_;
};

TEST(ParserTest, ParseInfixSymbol) {
  Parser parser;
  parser.AddSymbol("+", &InfixSymbol::Create<PlusSymbol>, PlusSymbol::BINDING_POWER);
  SymbolRef symbol;

  {
    TokenStream token_stream("1+2");
    EXPECT_TRUE(parser.Parse(&token_stream, &symbol));

    const PlusSymbol* plusSymbol = dynamic_cast<const PlusSymbol*>(symbol.ptr());
    EXPECT_NOT_NULL(plusSymbol);

    const Symbol* left = plusSymbol->left();
    EXPECT_EQ(left->value(), "1");

    const Symbol* right = plusSymbol->right();
    EXPECT_EQ(right->value(), "2");
  }

  {
    TokenStream token_stream("+2");
    EXPECT_FALSE(parser.Parse(&token_stream, &symbol));
  }

  {
    TokenStream token_stream("1+");
    EXPECT_FALSE(parser.Parse(&token_stream, &symbol));
  }
}


class TestMemPrefixSymbol : public PrefixSymbol,
                            public testing::InstCount<TestMemPrefixSymbol> {
public:
  TestMemPrefixSymbol(const Token* token) : PrefixSymbol(token), testing::InstCount<TestMemPrefixSymbol>() {}

  bool Parse(Parser* parser, SymbolRef* root) {
    *root = this;
    return true;
  }
};

class TestMemInfixSymbol : public InfixSymbol,
                           public testing::InstCount<TestMemInfixSymbol> {
public:
  static const uint BINDING_POWER = 10;

  TestMemInfixSymbol(const Token* token) : InfixSymbol(token), testing::InstCount<TestMemInfixSymbol>() {}

  bool Parse(Parser* parser, const Symbol* left, SymbolRef* root) {
    *root = this;
    return true;
  }
};

TEST(ParserTest, MemoryManagement) {
  {
    EXPECT_EQ(TestMemPrefixSymbol::inst_count(), 0);
    EXPECT_EQ(TestMemInfixSymbol::inst_count(), 0);

    Parser parser;
    parser.AddSymbol("a", &PrefixSymbol::Create<TestMemPrefixSymbol>);
    parser.AddSymbol("b", &InfixSymbol::Create<TestMemInfixSymbol>, TestMemInfixSymbol::BINDING_POWER);

    TokenStream token_stream("a b a b a");
    SymbolRefList symbols;
    EXPECT_TRUE(parser.Parse(&token_stream, &symbols));
    EXPECT_EQ(symbols.size(), 3);

    EXPECT_EQ(TestMemPrefixSymbol::inst_count(), 1);
    EXPECT_EQ(TestMemInfixSymbol::inst_count(), 2);

    const Symbol* symbol = symbols[0].ptr();
    EXPECT_EQ(symbol->value(), "b");
    EXPECT_NOT_NULL(dynamic_cast<const TestMemInfixSymbol*>(symbol));

    symbol = symbols[1].ptr();
    EXPECT_EQ(symbol->value(), "b");
    EXPECT_NOT_NULL(dynamic_cast<const TestMemInfixSymbol*>(symbol));

    symbol = symbols[2].ptr();
    EXPECT_EQ(symbol->value(), "a");
    EXPECT_NOT_NULL(dynamic_cast<const TestMemPrefixSymbol*>(symbol));
  }

  EXPECT_EQ(TestMemPrefixSymbol::inst_count(), 0);
  EXPECT_EQ(TestMemInfixSymbol::inst_count(), 0);
}

class ConsumePrefixSymbol : public PrefixSymbol {
public:
  ConsumePrefixSymbol(const Token* token) : PrefixSymbol(token) {}

  bool Parse(Parser* parser, SymbolRef* root) {
    if (!parser->Consume(";"))
      return false;

    *root = this;
    return true;
  }
};

TEST(ParserTest, ConsumeTest) {
  Parser parser;
  parser.AddSymbol("a", &PrefixSymbol::Create<ConsumePrefixSymbol>);

  TokenStream token_stream("a;");
  SymbolRefList symbols;
  EXPECT_TRUE(parser.Parse(&token_stream, &symbols));
  EXPECT_EQ(symbols.size(), 1);
}

class PeekPrefixSymbol : public PrefixSymbol {
public:
  PeekPrefixSymbol(const Token* token) : PrefixSymbol(token) {}

  bool Parse(Parser* parser, SymbolRef* root) {
    if (parser->PeekNextValue() == "a" && !parser->Expression(&right_))
      return false;

    *root = this;
    return true;
  }

  const Symbol* right() const { return right_.ptr(); }

private:
  SymbolRef right_;
};

TEST(ParserTest, PeekNextValue) {
  Parser parser;
  parser.AddSymbol("a", &PrefixSymbol::Create<PeekPrefixSymbol>);

  TokenStream token_stream("a a");
  SymbolRefList symbols;
  EXPECT_TRUE(parser.Parse(&token_stream, &symbols));
  EXPECT_EQ(symbols.size(), 1);

  const PeekPrefixSymbol* symbol = dynamic_cast<const PeekPrefixSymbol*>(symbols[0].ptr());
  EXPECT_NOT_NULL(symbol);
  EXPECT_NOT_NULL(symbol->right());

  symbol = dynamic_cast<const PeekPrefixSymbol*>(symbol->right());
  EXPECT_NOT_NULL(symbol);
  EXPECT_NULL(symbol->right());
}

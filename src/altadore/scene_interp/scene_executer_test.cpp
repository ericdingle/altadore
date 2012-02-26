#include "altadore/scene_interp/scene_executer.h"

#include "altadore/algebra/point3.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene_interp/scene_parser.h"
#include "altadore/shader/color.h"
#include "altadore/util/invokable.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"

class TestSceneExecuter : public SceneExecuter {
 public:
  TestSceneExecuter(Parser* parser) : SceneExecuter(parser) {
  }

  void Set(const std::string& name, const Variant* var) {
    var_map_[name] = var;
  }
};

class Object : public Invokable {
 public:
  virtual Result Invoke(
      const std::string& name,
      const std::vector<memory::scoped_refptr<const Variant> >& args,
      const Variant** var) {
    memory::scoped_refptr<const Variant> var_ref(new Variant(5));
    *var = var_ref.Release();
    return name == "pass" ? RESULT_OK : RESULT_ERR_NAME;
  }
};

TEST_CASE(SceneExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new SceneParser(stream_.ptr()));
    executer_.Reset(new TestSceneExecuter(parser_.ptr()));
  }

  SceneLexer lexer_;
  memory::scoped_ptr<TokenStream> stream_;
  memory::scoped_ptr<Parser> parser_;
  memory::scoped_ptr<TestSceneExecuter> executer_;
  memory::scoped_refptr<const Variant> var_;
};

TEST(SceneExecuterTest, ExecuteDotAccessor) {
  Init("obj.pass(1);");

  memory::scoped_refptr<Invokable> object(new Object());
  var_.Reset(new Variant(object.ptr()));
  executer_->Set("obj", var_.ptr());

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(i, 5);
}

TEST(SceneExecuterTest, ExecuteDotAccessorError) {
  Init("obj.pass(1);");

  var_.Reset(new Variant(5));
  executer_->Set("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.pass(a);");

  Invokable* object = new Object();
  var_.Reset(new Variant(object));
  executer_->Set("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("obj.fail(1);");

  object = new Object();
  var_.Reset(new Variant(object));
  executer_->Set("obj", var_.ptr());

  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteAssignment) {
  Init("a=1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(d, 1);
}

TEST(SceneExecuterTest, ExecuteAssignmentError) {
  Init("a=b;");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteIdentifier) {
  Init("a;");

  var_.Reset(new Variant(5));
  executer_->Set("a", var_.ptr());

  int i;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(i, 5);
}

TEST(SceneExecuterTest, ExecuteIdentifierError) {
  Init("a;");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNew) {
  Init("new Color();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  memory::scoped_refptr<Invokable> object;
  EXPECT_TRUE(var_->Get(object.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Color*>(object.ptr()));

  Init("new Point3();");
  EXPECT_TRUE(executer_->Execute(var_.Receive()));
  EXPECT_TRUE(var_->Get(object.Receive()));
  EXPECT_NOT_NULL(dynamic_cast<Point3*>(object.ptr()));
}

TEST(SceneExecuterTest, ExecuteNewError) {
  Init("new Color(a);");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());

  Init("new Blah();");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(SceneExecuterTest, ExecuteNumber) {
  Init("1;");
  double d;
  EXPECT_TRUE(executer_->ExecuteT(&d));
  EXPECT_EQ(d, 1);
}

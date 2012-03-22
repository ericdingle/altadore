#include "altadore/ray_tracer/ray_tracer.h"
#include "altadore/scene/transform_node.h"
#include "altadore/scene_interp/scene_executer.h"
#include "altadore/scene_interp/scene_lexer.h"
#include "altadore/scene_interp/scene_parser.h"
#include "altadore/shader/light_vector.h"
#include "bonavista/base/command_line.h"
#include "bonavista/file/util.h"
#include "bonavista/memory/scoped_refptr.h"
#include "chaparral/lexer/token_stream.h"

int main(int argc, char* argv[]) {
  CommandLine cmd_line(argc, argv);

  std::string input_file = cmd_line.GetFlag("input-file");
  if (input_file.empty()) {
    printf("Missing --input-file flag.\n");
    return 1;
  }

  std::string output_file = cmd_line.GetFlag("output-file");
  if (output_file.empty()) {
    printf("Missing --output-file flag.\n");
    return 1;
  }

  std::string input;
  if (!ReadFile(input_file.c_str(), &input)) {
    printf("Could not input read file.\n");
    return 1;
  }

  SceneLexer lexer;
  TokenStream stream(&lexer, input);
  SceneParser parser(&stream);
  SceneExecuter executer(&parser);

  scoped_refptr<TransformNode> root(new TransformNode());
  executer.SetVar("root", new Variant(dynamic_cast<Invokable*>(root.ptr())));

  scoped_refptr<LightVector> lights(new LightVector());
  executer.SetVar("lights", new Variant(dynamic_cast<Invokable*>(lights.ptr())));

  if (!executer.ExecuteAll()) {
    printf("Error: %s at line %d, column %d\n", executer.error().c_str(),
           executer.position().line, executer.position().column);
    return 1;
  }

  root->CalculateTransforms(Matrix4());
  RayTracer ray_tracer(root.ptr(), lights.ptr());
  if (!ray_tracer.Render(output_file.c_str(), 600, 400, false)) {
    printf("Could not render!");
    return 1;
  }

  return 0;
}

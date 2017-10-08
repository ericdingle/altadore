#include <fstream>
#include <memory>
#include <unistd.h>
#include "ray_tracer/ray_tracer.h"
#include "scene/transform_node.h"
#include "scene_interp/scene_executer.h"
#include "scene_interp/scene_lexer.h"
#include "scene_interp/scene_parser.h"
#include "shader/light_vector.h"
#include "third_party/bonavista/src/lexer/token_stream.h"

int main(int argc, char* argv[]) {
  // Parse flags.
  std::string input_file, output_file;
  int width = 800, height = 600;
  bool anti_alias = false;

  opterr = 0;
  for (int c = 0; c != -1; c = getopt(argc, argv, "i:o:w:h:a")) {
    switch (c) {
      case 'i':
        input_file = optarg;
        break;
      case 'o':
        output_file = optarg;
        break;
      case 'w':
        width = atoi(optarg);
        break;
      case 'h':
        height = atoi(optarg);
        break;
      case 'a':
        anti_alias = true;
        break;
      case '?':
        printf("Unsupported option: %c\n", optopt);
        return 1;
    }
  }

  // Check flags.
  if (input_file.empty()) {
    printf("Missing input file flag.\n");
    return 1;
  }

  if (output_file.empty()) {
    size_t pos = input_file.rfind('.');
    output_file = input_file.substr(0, pos) + ".bmp";
  }

  if (width == 0) {
    printf("Invalid width.\n");
    return 1;
  }

  if (height == 0) {
    printf("Invalid height.\n");
    return 1;
  }

  // Read input file.
  std::ifstream input_stream(input_file);
  std::string input((std::istreambuf_iterator<char>(input_stream)),
                    (std::istreambuf_iterator<char>()));

  // Execute scene.
  SceneLexer lexer;
  TokenStream stream(&lexer, input);
  SceneParser parser(&stream);
  SceneExecuter executer(&parser);

  std::shared_ptr<TransformNode> root(new TransformNode());
  executer.SetVar("root", std::make_shared<Variant>(
      std::dynamic_pointer_cast<Invokable>(root)));

  std::shared_ptr<LightVector> lights(new LightVector());
  executer.SetVar("lights", std::make_shared<Variant>(
      std::dynamic_pointer_cast<Invokable>(lights)));

  if (!executer.ExecuteAll()) {
    printf("Error: %s at line %d, column %d\n", executer.error().c_str(),
           executer.position().line, executer.position().column);
    return 1;
  }

  // Render scene.
  root->CalculateTransforms(Matrix4());
  RayTracer ray_tracer(root, lights);
  if (!ray_tracer.Render(output_file.c_str(), width, height, anti_alias)) {
    printf("Could not render!");
    return 1;
  }

  return 0;
}

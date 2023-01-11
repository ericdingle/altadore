package(default_visibility = ["//visibility:public"])

cc_library(
    name = "scene_executer",
    hdrs = [
        "scene_executer.h",
        "scene_object.h",
    ],
    srcs = [
        "scene_executer.cpp",
        "scene_object.cpp",
    ],
    deps = [
        ":scene_lexer",
        "//altadore/scene:shape_node",
        "//altadore/scene:transform_node",
        "//altadore/shader:light",
        "//altadore/shape:cube",
        "//altadore/shape:sphere",
        "@bonavista//bonavista/executer",
        "@bonavista//bonavista/lexer:token",
        "@bonavista//bonavista/parser:node",
        "@com_google_absl//absl/status",
    ],
)

cc_test(
    name = "scene_executer_test",
    srcs = ["scene_executer_test.cpp"],
    deps = [
        ":scene_executer",
        ":scene_lexer",
        ":scene_parser",
        "//altadore/algebra:matrix4",
        "@bonavista//bonavista/executer:executer_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "scene_lexer",
    hdrs = ["scene_lexer.h"],
    srcs = ["scene_lexer.cpp"],
    deps = [
        "@bonavista//bonavista/lexer",
        "@bonavista//bonavista/util:status_macros",
    ],
)

cc_test(
    name = "scene_lexer_test",
    srcs = ["scene_lexer_test.cpp"],
    deps = [
        ":scene_lexer",
        "@bonavista//bonavista/lexer:lexer_test_fixture",
        "@bonavista//bonavista/lexer:token_test_macros",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "scene_object_test",
    srcs = ["scene_object_test.cpp"],
    deps = [
        ":scene_executer",
        ":scene_parser",
        "@bonavista//bonavista/executer:executer_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "scene_parser",
    hdrs = ["scene_parser.h"],
    srcs = ["scene_parser.cpp"],
    deps = [
        ":scene_lexer",
        "@bonavista//bonavista/parser",
        "@bonavista//bonavista/util:status_macros",
    ],
)

cc_test(
    name = "scene_parser_test",
    srcs = ["scene_parser_test.cpp"],
    deps = [
        ":scene_parser",
        "@bonavista//bonavista/lexer:token_test_macros",
        "@bonavista//bonavista/parser:parser_test_fixture",
        "@com_google_googletest//:gtest_main",
    ],
)

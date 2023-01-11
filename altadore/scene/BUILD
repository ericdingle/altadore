package(default_visibility = ["//visibility:public"])

cc_library(
    name = "scene_node",
    hdrs = ["scene_node.h"],
    deps = ["//altadore/algebra:ray"],
)

cc_library(
    name = "shape_node",
    hdrs = ["shape_node.h"],
    srcs = ["shape_node.cpp"],
    deps = [
        ":scene_node",
        "//altadore/algebra:matrix4",
        "//altadore/shader:material",
        "//altadore/shape",
    ],
)

cc_test(
    name = "shape_node_test",
    srcs = ["shape_node_test.cpp"],
    deps = [
        ":shape_node",
        "//altadore/shape:cube",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "transform_node",
    hdrs = ["transform_node.h"],
    srcs = ["transform_node.cpp"],
    deps = [
        ":scene_node",
        "//altadore/algebra:matrix4",
    ],
)

cc_test(
    name = "transform_node_test",
    srcs = ["transform_node_test.cpp"],
    deps = [
        ":transform_node",
        "@com_google_googletest//:gtest_main",
    ],
)

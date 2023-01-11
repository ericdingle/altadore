package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cube",
    hdrs = ["cube.h"],
    srcs = ["cube.cpp"],
    deps = [
        ":shape",
        ":shape_constants",
        "//altadore/algebra:ray",
    ],
)

cc_test(
    name = "cube_test",
    srcs = ["cube_test.cpp"],
    deps = [
        ":cube",
        "//altadore/algebra:ray",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "shape",
    hdrs = ["shape.h"],
)

cc_library(
    name = "shape_constants",
    hdrs = ["shape_constants.h"],
    srcs = ["shape_constants.cpp"],
)

cc_library(
    name = "sphere",
    hdrs = ["sphere.h"],
    srcs = ["sphere.cpp"],
    deps = [
        ":shape",
        ":shape_constants",
        "//altadore/algebra:ray",
    ],
)

cc_test(
    name = "sphere_test",
    srcs = ["sphere_test.cpp"],
    deps = [
        ":sphere",
        "//altadore/algebra:ray",
        "@com_google_googletest//:gtest_main",
    ],
)

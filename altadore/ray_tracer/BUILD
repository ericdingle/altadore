package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ray_tracer_constants",
    hdrs = ["ray_tracer_constants.h"],
    srcs = ["ray_tracer_constants.cpp"],
)

cc_library(
    name = "ray_tracer",
    hdrs = ["ray_tracer.h"],
    srcs = ["ray_tracer.cpp"],
    deps = [
        ":ray_tracer_constants",
        "//altadore/image:bitmap",
        "//altadore/scene:transform_node",
        "//altadore/shader:light",
        "//altadore/shader:material",
    ],
)

cc_test(
    name = "ray_tracer_test",
    srcs = ["ray_tracer_test.cpp"],
    deps = [
        ":ray_tracer",
        "@com_google_googletest//:gtest_main",
    ],
)

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "algebra_constants",
    hdrs = ["algebra_constants.h"],
    srcs = ["algebra_constants.cpp"],
)

cc_library(
    name = "matrix4",
    hdrs = ["matrix4.h"],
    srcs = ["matrix4.cpp"],
    deps = [
        ":algebra_constants",
        ":point3",
        ":vector3",
    ],
)

cc_test(
    name = "matrix4_test",
    srcs = ["matrix4_test.cpp"],
    deps = [
        ":matrix4",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "point3",
    hdrs = ["point3.h"],
    srcs = ["point3.cpp"],
    deps = [":vector3"],
)

cc_test(
    name = "point3_test",
    srcs = ["point3_test.cpp"],
    deps = [
        ":point3",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "ray",
    hdrs = ["ray.h"],
    srcs = ["ray.cpp"],
    deps = [
        ":point3",
        ":vector3",
    ],
)

cc_test(
    name = "ray_test",
    srcs = ["ray_test.cpp"],
    deps = [
        ":ray",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "vector3",
    hdrs = ["vector3.h"],
    srcs = ["vector3.cpp"],
)

cc_test(
    name = "vector3_test",
    srcs = ["vector3_test.cpp"],
    deps = [
        ":vector3",
        "@com_google_googletest//:gtest_main",
    ],
)

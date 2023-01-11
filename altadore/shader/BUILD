package(default_visibility = ["//visibility:public"])

cc_library(
    name = "color",
    hdrs = ["color.h"],
    srcs = ["color.cpp"],
)

cc_test(
    name = "color_test",
    srcs = ["color_test.cpp"],
    deps = [
        ":color",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "light",
    hdrs = ["light.h"],
    srcs = ["light.cpp"],
    deps = [
        ":color",
        "//altadore/algebra:point3",
        "//altadore/algebra:vector3",
    ],
)

cc_test(
    name = "light_test",
    srcs = ["light_test.cpp"],
    deps = [
        ":light",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "material",
    hdrs = ["material.h"],
    srcs = ["material.cpp"],
    deps = [":color"],
)

cc_test(
    name = "material_test",
    srcs = ["material_test.cpp"],
    deps = [
        ":material",
        "@com_google_googletest//:gtest_main",
    ],
)

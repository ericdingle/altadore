package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bitmap",
    hdrs = ["bitmap.h"],
    srcs = ["bitmap.cpp"],
)

cc_test(
    name = "bitmap_test",
    srcs = ["bitmap_test.cpp"],
    deps = [
        ":bitmap",
        "@com_google_googletest//:gtest_main",
    ],
)

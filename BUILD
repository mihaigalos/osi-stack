load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "sut",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    copts = ["-std=c++17"],
    strip_include_prefix = "include",
)

cc_test(
    name = "unit_tests",
    srcs = glob([
        "test/**/*.cpp",
        "test/**/*.h",
    ]),
    copts = [
        "-fdiagnostics-color",
        "-std=c++17",
    ],
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

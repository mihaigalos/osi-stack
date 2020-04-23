load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

DEFAULT_COMPILER_OPTIONS = [
    "-fdiagnostics-color",
    "-DTESTING",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
    "-Werror",
    "-std=c++17",
]

cc_library(
    name = "sut",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    copts = DEFAULT_COMPILER_OPTIONS,
    strip_include_prefix = "include",
)

cc_test(
    name = "unit_logic_arrays_trx",
    srcs = [
        "test/unit/logic_arrays_trx.cpp",
    ],
    copts = DEFAULT_COMPILER_OPTIONS,
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "unit_logic_reset_payload",
    srcs = [
        "test/unit/logic_reset_payload.cpp",
    ],
    copts = DEFAULT_COMPILER_OPTIONS,
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "unit_logic_sanity",
    srcs = [
        "test/unit/logic_sanity.cpp",
    ],
    copts = DEFAULT_COMPILER_OPTIONS,
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "unit_logic_single_bytes_trx",
    srcs = [
        "test/unit/logic_single_bytes_trx.cpp",
    ],
    copts = DEFAULT_COMPILER_OPTIONS,
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "sut",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    copts = ["-std=c++17"],
    strip_include_prefix = "include",
)

cc_test(
    name = "unit_logic_arrays_trx",
    srcs = [
        "test/unit_logic_arrays_trx.cpp",
    ],
    copts = [
        "-fdiagnostics-color",
        "-DTESTING",
        "-std=c++17",
    ],
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
        "test/unit_logic_reset_payload.cpp",
    ],
    copts = [
        "-fdiagnostics-color",
        "-DTESTING",
        "-std=c++17",
    ],
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
        "test/unit_logic_sanity.cpp",
    ],
    copts = [
        "-fdiagnostics-color",
        "-DTESTING",
        "-std=c++17",
    ],
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
        "test/unit_logic_single_bytes_trx.cpp",
    ],
    copts = [
        "-fdiagnostics-color",
        "-DTESTING",
        "-std=c++17",
    ],
    tags = ["unit"],
    deps = [
        ":sut",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)

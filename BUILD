load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

DEFAULT_COMPILER_OPTIONS = [
    "-DLOGGING",
    "-DTESTING",
    "-fdiagnostics-color",
    "-fmax-errors=3",
    "-std=c++14",
    "-Wall",
    "-Werror",
    "-Wextra",
    "-Wpedantic",
]

DEFAULT_TEST_DEPS = [
    ":osi_stack",
    "@gtest",
    "@gtest//:gtest_main",
]

DEFAULT_TEST_COMPILE_OPTIONS = DEFAULT_COMPILER_OPTIONS + [
    "-fsanitize=address",
    "-DADDRESS_SANITIZER",
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
]

DEFAULT_TEST_LINK_OPTIONS = [
    "-fsanitize=address",
    "-static-libasan",
]

cc_library(
    name = "osi_stack",
    srcs = glob(["src/**/*.cpp"]),
    hdrs = glob(["include/**/*.h"]),
    copts = DEFAULT_COMPILER_OPTIONS,
    strip_include_prefix = "include",
    deps = ["@containers"],
)

cc_library(
    name = "test_headers",
    hdrs = glob(["test/**/*.h"]),
    copts = DEFAULT_TEST_COMPILE_OPTIONS,
    linkopts = DEFAULT_TEST_LINK_OPTIONS,
    strip_include_prefix = "test",
)

[
    cc_test(
        name = "unit/" + unit_name,
        srcs = [
            "test/unit/" + unit_name + ".cpp",
        ],
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["unit"],
        deps = DEFAULT_TEST_DEPS + [":test_headers"],
    )
    for unit_name in [
        file_name.replace("test/unit/", "").replace(".cpp", "")
        for file_name in glob(["test/unit/**/*.cpp"])
    ]
]

[
    cc_test(
        name = "integration/" + integration_name,
        srcs = [
            "test/integration/" + integration_name + ".cpp",
        ],
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["integration"],
        deps = DEFAULT_TEST_DEPS + [":test_headers"],
    )
    for integration_name in [
        file_name.replace("test/integration/", "").replace(".cpp", "")
        for file_name in glob(["test/integration/**/*.cpp"])
    ]
]

[
    cc_test(
        name = "component/" + component_name,
        srcs = [
            "test/component/" + component_name + ".cpp",
        ],
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["component"],
        deps = DEFAULT_TEST_DEPS + [":test_headers"],
    )
    for component_name in [
        file_name.replace("test/component/", "").replace(".cpp", "")
        for file_name in glob(["test/component/**/*.cpp"])
    ]
]

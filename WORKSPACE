load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-master",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/master.zip"],
)

git_repository(
    name = "gtest",
    commit = "0eea2e9fc63461761dea5f2f517bd6af2ca024fa",
    remote = "https://github.com/google/googletest",
)

git_repository(
    name = "containers",
    commit = "5e448e93a54dcb72910244211b75151340c31446",
    remote = "https://github.com/mihaigalos/containers",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-master",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/master.zip"],
)

git_repository(
    name = "gtest",
    commit = "13a433a94dd9c7e55907d7a9b75f44ff82f309eb",
    remote = "https://github.com/google/googletest",
)

git_repository(
    name = "containers",
    commit = "707821b5f4dc19be05827a548ddbebe90f542852",
    remote = "https://github.com/mihaigalos/containers",
)

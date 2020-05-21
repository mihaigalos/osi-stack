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
    commit = "146821094f8b26eee3b453f1989d69237ef4dced",
    remote = "https://github.com/mihaigalos/containers",
)

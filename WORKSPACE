load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

http_archive(
  name = "rules_foreign_cc",
  sha256 = "484fc0e14856b9f7434072bc2662488b3fe84d7798a5b7c92a1feb1a0fa8d088",
  strip_prefix = "rules_foreign_cc-0.8.0",
  url = "https://primihub.oss-cn-beijing.aliyuncs.com/tools/rules_foreign_cc_cn-0.8.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

new_git_repository(
  name = "gmssl",
  build_file = "//bazel:BUILD.gmssl",
  remote = "https://gitee.com/mirrors/GmSSL.git",
  branch = "master"
)

http_archive(
  name = "fmtlib",
  sha256 = "6cb1e6d37bdcb756dbbe59be438790db409cdb4868c66e888d5df9f13f7c027f",
  strip_prefix = "fmt-11.0.2",
  build_file = "//bazel:BUILD.fmtlib",
  urls = [
    "https://primihub.oss-cn-beijing.aliyuncs.com/tools/fmt-11.0.2.tar.gz",
  ]
)

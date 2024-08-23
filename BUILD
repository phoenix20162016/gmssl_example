cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    "//util:log_util",
    "//util:common",
    "@gmssl//:gmssl",
    "@fmtlib//:fmt",
  ],
)

cc_binary(
  name = "sm4_main",
  srcs = ["demo_sm4_cbc.cc"],
  deps = [
    "//util:log_util",
    "//util:common",
    "@gmssl//:gmssl",
    "@fmtlib//:fmt",
  ],
)
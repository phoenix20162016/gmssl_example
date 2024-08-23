TARGET = //:main \
         //:sm4_main

target:
	bazel build ${TARGET}

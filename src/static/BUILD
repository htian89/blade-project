cc_library(
  name = "b",
  srcs = [
    'b.cc'
  ],
)

cc_library(
  name = "c",
  srcs = [
    'c.cc'
  ],
)

cc_binary(
  dynamic_link=True,
  name = "test",
  srcs = [
    'main.cc'
  ],
  deps = [
    ':b',
    ':c',
  ],
)

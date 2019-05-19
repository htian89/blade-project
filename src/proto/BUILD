proto_library(
    name = 'my_message',
    srcs = [
        'my_message.proto',
    ],
)

cc_binary(
  name = 'pb_test',
  srcs = [
    'main.cc'
  ],
  deps = [
    ':my_message',
  ],
)

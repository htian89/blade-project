// Copyright (c) 2018 JD.com Inc.
// Author: Tian hao (tianhao3@jd.com)

#include <gtest/gtest.h>
#include "toft/storage/recordio/recordio.h"
#include <glog/logging.h>
#include "toft/base/string/number.h"
#include "toft/base/scoped_ptr.h"
#include "src/proto/my_message.pb.h"

int main(int argc, char const* argv[])
{
  std::string file_path = argv[1];
  std::cout << file_path << std::endl;

  uint32_t compress_type = 2;
  if (argc >= 3) {
    compress_type = static_cast<uint32_t>(atoi(argv[2]));
    std::cout << compress_type << std::endl;
  }

  ::toft::scoped_ptr< ::toft::File> file(::toft::File::Open(file_path, "rb"));
  ::toft::scoped_ptr< ::toft::RecordReader> reader(new ::toft::RecordReader(
      file.get(), (::toft::CompressType)compress_type));

  do {
    ::MyPb::Message* message = new ::MyPb::Message;
    if (!reader->ReadNextMessage(message)) {
      delete message;
      break;
    }
    std::string buff = message->Utf8DebugString();
    delete message;
    std::cout << buff << std::endl;
  } while (true);
  file->Close();
  return 0;
}

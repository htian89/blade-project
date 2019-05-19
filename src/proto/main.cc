#include <string>
#include <iostream>
#include "src/proto/my_message.pb.h"

int main(int argc, char const* argv[])
{
  MyPb::Message m;
  for (size_t i = 0; i < 5; ++i) {
    MyPb::Data* d = m.add_datas();
    d->set_a(i);
  }
  MyPb::Message m_s;
  for (size_t i = 5; i < 10; ++i) {
    MyPb::Data* d = m_s.add_datas();
    d->set_a(i);
  }
  MyPb::Message m2;
  for (int i = 0; i < m.datas_size(); ++i) {
    m2.add_datas()->Swap(m.mutable_datas(i));
  }
  std::cout << m.Utf8DebugString()<< std::endl;
  for (int i = 0; i < m_s.datas_size(); ++i) {
    m2.add_datas()->Swap(m_s.mutable_datas(i));
  }
  std::cout << m_s.Utf8DebugString()<< std::endl;
  //m2.mutable_datas()->MergeFrom(m_s.datas());
  std::cout << m2.Utf8DebugString()<< std::endl;
  std::string str_m2;
  m2.SerializeToString(&str_m2);
  MyPb::Message m3;
  m3.ParseFromArray(str_m2.c_str(), str_m2.size());
  //std::cout << m2.Utf8DebugString()<< std::endl;
  return 0;
}

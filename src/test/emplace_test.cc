#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "toft/system/time/timestamp.h"

// Book结构，保存书本信息
struct SBook {
  SBook() : bookName(""), price(0) {
    std::cout << "default construct: " << bookName << std::endl;
  }
  SBook(std::string bookName_, int price_)
      : bookName(bookName_), price(price_) {
    std::cout << "construct: " << bookName << std::endl;
  };
  SBook(const SBook& rhs) : bookName(rhs.bookName), price(rhs.price) {
    std::cout << "copy construct: " << bookName << std::endl;
  }
  SBook(SBook&& rhs) : bookName(rhs.bookName), price(rhs.price) {
    std::cout << "move construct: " << bookName << std::endl;
  }
  SBook& operator=(const SBook& rhs) {
    bookName = rhs.bookName;
    price = rhs.price;
    std::cout << "Copy: " << bookName << std::endl;
    return *this;
  }
  SBook& operator=(SBook&& rhs) {
    bookName = std::move(rhs.bookName);
    price = rhs.price;
    std::cout << "Move: " << bookName << std::endl;
    return *this;
  }
  SBook& Clear() {
    bookName.clear();
    price = 0;
    return *this;
  }
  ~SBook() { std::cout << "deconstruct: " << bookName << std::endl; }

  bool operator<(const SBook& rhs) const { return bookName < rhs.bookName; }
  std::string bookName;
  int price;
};

TEST(TEST, Move) {
  SBook book("abcbef", 1);
  SBook book2;
  std::cout << std::endl;
  book2 = book;
  std::cout << std::endl;
  book2.Clear() = std::move(book);
  std::cout << "after move: " << book.bookName << std::endl;
  std::cout << std::endl;
  book2.Clear() = (book);
  std::cout << std::endl;
  SBook book3((book));
  std::cout << std::endl;
  SBook book4(std::move(book));
  std::cout << std::endl;
  SBook book5(book);
  std::cout << std::endl;
}

TEST(TEST, VectorEmplaceTest) {
  // 测试vector
  std::vector<SBook> books;
  // 预先分配，否则整个vector在容量不够的情况下重新分配内存
  books.reserve(100);
  SBook book1("红楼梦", 10);

  std::cout << "test push_back:" << std::endl;
  books.push_back(SBook("C++从入门到放弃", 1));
  std::cout << std::endl;

  std::cout << "test push_back: with move" << std::endl;
  books.push_back(std::move(SBook("C++从入门到放弃", 1)));
  std::cout << std::endl;

  std::cout << "test emplace_back lvalue:" << std::endl;
  books.emplace_back(book1);
  std::cout << std::endl;

  std::cout << "test emplace_back lvalue: with move" << std::endl;
  books.emplace_back(std::move(book1));
  std::cout << std::endl;

  std::cout << "test emplace_back rvalue:" << std::endl;
  books.emplace_back(SBook("水浒传", 2));
  std::cout << std::endl;

  std::cout << "test emplace_back rvalue:with move" << std::endl;
  books.emplace_back(std::move(SBook("水浒传", 2)));
  std::cout << std::endl;

  std::cout << "test emplace_back:" << std::endl;
  books.emplace_back("水浒传", 2);
  std::cout << std::endl;

  std::cout << "test emplace_back default:" << std::endl;
  books.emplace_back();
  auto& book = books.back();
  book.bookName = "红楼梦";
  book.price = 5;
  std::cout << std::endl;

  std::cout << "test emplace:" << std::endl;
  auto it = books.emplace(books.end());
  it->bookName = "西游记";
  it->price = 3;
  std::cout << std::endl;

  std::cout << "output all books: " << std::endl;
  std::for_each(books.begin(), books.end(), [](const SBook& book)->void
           {
           std::cout << book.bookName << std::endl;
           });
  std::cout << std::endl;
}

TEST(TEST, SetEmplaceTest) {
  // 测试set
  std::set<SBook> bookSet;
  std::cout << "test bookSet insert:" << std::endl;
  bookSet.insert(SBook("十万个为什么", 1));
  std::cout << std::endl;
  std::cout<< "test bookSet emplace:" << std::endl;
  bookSet.emplace("新华字典", 2);
  std::cout << std::endl;
  std::cout<< "output bookset: " << std::endl;
  std::for_each(bookSet.begin(), bookSet.end(), [](const SBook&book)->void
           {
           std::cout << book.bookName << std::endl;
           });
  std::cout << std::endl;
}

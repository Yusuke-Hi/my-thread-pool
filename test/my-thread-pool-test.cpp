#include "my-thread-pool.hpp"

#include <gtest/gtest.h>

#include <iostream>

const size_t kThreadSize{100};

TEST(MyThreadPoolTest, Basic) {
  MyThreadPool::Init(kThreadSize);
  auto& pool = MyThreadPool::GetInstance();
  pool.Submit([]() {
    sleep(1);
    std::cout << "Hello Basic World." << std::endl;
  });
  pool.Submit([]() { std::cout << "Hello Basic Contry." << std::endl; });
  MyThreadPool::ResetForTest();
}

TEST(MyThreadPoolTest, WaitAll) {
  MyThreadPool::Init(kThreadSize);
  auto& pool = MyThreadPool::GetInstance();
  pool.Submit([]() {
    sleep(1);
    std::cout << "Hello WaitAll World." << std::endl;
  });
  pool.Submit([]() { std::cout << "Hello WaitAll Contry." << std::endl; });
  pool.WaitAll();
  MyThreadPool::ResetForTest();
}

TEST(MyThreadPoolTest, Future) {
  MyThreadPool::Init(kThreadSize);
  auto& pool = MyThreadPool::GetInstance();
  auto future_int = pool.Submit([]() { return 42; });
  auto future_string = pool.Submit([]() { return "Hello World."; });
  int result_int = future_int.get();
  std::string result_string = future_string.get();
  EXPECT_EQ(result_int, 42);
  EXPECT_EQ(result_string, "Hello World.");
  MyThreadPool::ResetForTest();
}

TEST(MyThreadPoolTest, GetInstanceFail) {
  EXPECT_THROW(auto& pool = MyThreadPool::GetInstance(), std::logic_error);
  MyThreadPool::ResetForTest();
}

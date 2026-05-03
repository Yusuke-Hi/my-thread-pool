#include "my-thread-pool.hpp"

#include <gtest/gtest.h>

#include <iostream>

TEST(MyThreadPoolTest, Basic) {
  auto& pool = MyThreadPool::GetInstance();
  pool.Submit([]() {
    sleep(1);
    std::cout << "Hello Basic World." << std::endl;
  });
  pool.Submit([]() { std::cout << "Hello Basic Contry." << std::endl; });
}

TEST(MyThreadPoolTest, WaitAll) {
  auto& pool = MyThreadPool::GetInstance();
  pool.Submit([]() {
    sleep(1);
    std::cout << "Hello WaitAll World." << std::endl;
  });
  pool.Submit([]() { std::cout << "Hello WaitAll Contry." << std::endl; });
  pool.WaitAll();
}

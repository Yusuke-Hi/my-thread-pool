#include "my-thread-pool.hpp"

#include <gtest/gtest.h>

#include <iostream>

TEST(MyThreadPoolTest, Basic) {
  auto& pool = MyThreadPool::GetInstance();
  pool.Submit([]() {
    sleep(1);
    std::cout << "Hello World." << std::endl;
  });
  pool.Submit([]() { std::cout << "Hello Contry." << std::endl; });
}

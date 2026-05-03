#include "my-thread-pool.hpp"

MyThreadPool::MyThreadPool() : stop_{false} {
  for (int i = 0; i < kNumThread; ++i) {
    std::thread t([this]() { this->WorkInThread(); });
    thread_vector_.push_back(std::move(t));
  }
}

MyThreadPool::~MyThreadPool() {
  {
    std::lock_guard<std::mutex> lock(mtx_);
    stop_ = true;
  }
  cv_.notify_all();
  for (int i = 0; i < kNumThread; ++i) {
    if (thread_vector_[i].joinable()) {
      thread_vector_[i].join();
    }
  }
}

void MyThreadPool::WorkInThread() {
  while (true) {
    std::unique_lock<std::mutex> lock{mtx_};
    cv_.wait(lock, [this]() { return stop_ || !task_queue_.empty(); });
    if (stop_) {
      return;
    }
    auto func = task_queue_.front();
    task_queue_.pop();
    lock.unlock();
    func();
  }
}

MyThreadPool& MyThreadPool::GetInstance() {
  static MyThreadPool instance;
  return instance;
}

void MyThreadPool::Submit(std::function<void()> func) {
  {
    std::lock_guard<std::mutex> lock(mtx_);
    task_queue_.push(std::move(func));
  }
  cv_.notify_one();
}

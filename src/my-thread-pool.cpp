#include "my-thread-pool.hpp"

MyThreadPool::MyThreadPool() : stop_{false}, running_func_count{0} {
  for (int i = 0; i < kNumThread; ++i) {
    std::thread t([this]() { this->WorkInThread(); });
    thread_vector_.push_back(std::move(t));
  }
}

MyThreadPool::~MyThreadPool() {
  stop_ = true;
  cv_thread.notify_all();
  for (int i = 0; i < kNumThread; ++i) {
    if (thread_vector_[i].joinable()) {
      thread_vector_[i].join();
    }
  }
}

void MyThreadPool::WorkInThread() {
  while (true) {
    std::unique_lock<std::mutex> lock{mtx_};
    cv_thread.wait(lock, [this]() { return stop_ || !task_queue_.empty(); });
    if (stop_) {
      return;
    }
    auto func = std::move(task_queue_.front());
    task_queue_.pop();
    lock.unlock();
    ++running_func_count;
    func();
    --running_func_count;
    cv_wait_all.notify_one();
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
  cv_thread.notify_one();
}

void MyThreadPool::WaitAll() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_wait_all.wait(lock, [this]() {
    return task_queue_.empty() && running_func_count == 0;
  });
}

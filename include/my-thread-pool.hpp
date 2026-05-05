#ifndef MY_THREAD_POOL
#define MY_THREAD_POOL

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

constexpr size_t kDefualtThreadNum{100};

class MyThreadPool {
 public:
  ~MyThreadPool();
  MyThreadPool(const MyThreadPool&) = delete;
  MyThreadPool& operator=(const MyThreadPool&) = delete;

  static void Init(const size_t thread_size) {
    if (instance) {
      std::cerr << "Init has already been called once.\n";
    }
    instance = std::unique_ptr<MyThreadPool>(new MyThreadPool(thread_size));
  }

  static MyThreadPool& GetInstance() {
    if (!instance.get()) {
      throw std::logic_error("You have to call Init before GetInstance.");
    }
    return *instance;
  }

  template <typename F, typename T = std::invoke_result_t<F>>
  std::future<T> Submit(F func) {
    std::promise<T> promise;
    std::future<T> future = promise.get_future();
    std::packaged_task<void()> task{[p = std::move(promise), func]() mutable {
      try {
        if constexpr (std::is_void_v<T>) {
          p.set_value();
          func();
        } else {
          p.set_value(func());
        }
      } catch (...) {
        p.set_exception(std::current_exception());
      }
    }};
    {
      std::lock_guard<std::mutex> lock(mtx_);
      task_queue_.emplace(std::move(task));
    }
    cv_thread.notify_one();
    return future;
  }

  void WaitAll();

  inline static void ResetForTest() { instance = nullptr; };

 private:
  MyThreadPool(const size_t thread_size);
  void WorkInThread();

  std::vector<std::thread> thread_vector_;
  std::queue<std::packaged_task<void()>> task_queue_;
  std::mutex mtx_;
  std::condition_variable cv_thread;
  std::condition_variable cv_wait_all;
  std::atomic<bool> stop_;
  std::atomic<int> running_func_count;
  const size_t kNumThread;
  static std::unique_ptr<MyThreadPool> instance;
};

#endif  // MY_THREAD_POOL

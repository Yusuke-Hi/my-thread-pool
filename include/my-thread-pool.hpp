#ifndef MY_THREAD_POOL
#define MY_THREAD_POOL

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

constexpr size_t kNumThread{100};

class MyThreadPool {
 public:
  ~MyThreadPool();
  MyThreadPool(const MyThreadPool&) = delete;
  MyThreadPool& operator=(const MyThreadPool&) = delete;

  static MyThreadPool& GetInstance();
  void Submit(std::function<void()> func);
  void WaitAll();

 private:
  MyThreadPool();
  void WorkInThread();

  std::vector<std::thread> thread_vector_;
  std::queue<std::function<void()>> task_queue_;
  std::mutex mtx_;
  std::condition_variable cv_thread;
  std::condition_variable cv_wait_all;
  std::atomic<bool> stop_;
  std::atomic<int> running_func_count;
};

#endif  // MY_THREAD_POOL

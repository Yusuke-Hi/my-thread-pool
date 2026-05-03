#ifndef MY_THREAD_POOL
#define MY_THREAD_POOL

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

 private:
  MyThreadPool();
  void WorkInThread();

  std::vector<std::thread> thread_vector_;
  std::queue<std::function<void()>> task_queue_;
  std::mutex mtx_;
  std::condition_variable cv_;
  bool stop_;
};

#endif  // MY_THREAD_POOL

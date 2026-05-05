# my-thread-pool
A simple thread pool implementation written in C++. This project was created to learn thread programming.

## Features
- **Thread Pool Initialization**  
  Creates a fixed number of worker threads at startup via `Init(size_t thread_size)`.
- **Task Submission with Future Support**  
  Submit any callable and retrieve its return value via `std::future`.
- **Wait for Completion**  
  `WaitAll()` blocks until all queued tasks have finished executing.
- **Singleton Design**  
  Only one instance exists per process, enforced at compile time.


## Build
```
mkdir build
cd build
cmake ..
make
```

## How to use
```
constexpr size_t kThreadSize{100};
MyThreadPool::Init(kThreadSize);
auto& pool = MyThreadPool::GetInstance();
pool.Submit([](){return 10 + 5;});
```

## Architecture
### Key Design Decisions
- **Singleton**: Only one thread pool per process.
- **condition_variable**: Worker threads sleep when idle, avoiding busy-loop CPU waste.
- **std::future / std::promise**: Task results are retrieved asynchronously without blocking the caller.
- **std::atomic**: `stop_` and `running_func_count` use atomics instead of mutex for lightweight synchronization.
- **Graceful Shutdown**: Destructor sets `stop_=true` and joins all threads after current tasks complete.

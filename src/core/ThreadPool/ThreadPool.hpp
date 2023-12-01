#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

using std::bind;
using std::condition_variable;
using std::forward;
using std::function;
using std::future;
using std::make_shared;
using std::mutex;
using std::packaged_task;
using std::queue;
using std::result_of;
using std::runtime_error;
using std::thread;
using std::unique_lock;
using std::vector;

class ThreadPool {
private:
  vector<thread> workers;
  queue<function<void()>> tasks;
  mutex queueMutex;
  condition_variable condition;
  bool stop;

public:
  ThreadPool(size_t threads) : stop(false) {
    for (int i = 0; i < threads; ++i) {
      workers.emplace_back([this]() {
        while (true) {
          function<void()> task;

          {
            unique_lock<mutex> lock(this->queueMutex);
            this->condition.wait(
                lock, [this]() { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty())
              return;
            task = move(this->tasks.front());
            tasks.pop();
          }
          task();
        }
      });
    }
  };
  template <class F, class... Args>
  auto enqueue(F &&f, Args &&...args)
      -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;

    auto task = make_shared<packaged_task<return_type()>>(
        bind(forward<F>(f), forward<Args>(args)...));

    future<return_type> res = task->get_future();

    {
      unique_lock<mutex> lock(queueMutex);

      if (stop)
        throw runtime_error("enqueue on stopped ThreadPool");

      tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_all();
    return res;
  }
  ~ThreadPool() {
    {
      unique_lock<mutex> lock(queueMutex);
      stop = true;
    }
    condition.notify_all();
    for (auto &worker : workers) {
      worker.join();
    }
  }
};

#endif // THREADPOOL_H
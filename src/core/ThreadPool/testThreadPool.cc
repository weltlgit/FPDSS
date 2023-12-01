#include <iostream>
#include <chrono>
#include "ThreadPool.hpp" // 假设你的线程池类定义在 "ThreadPool.h" 文件中

void exampleTask(int n) {
    std::cout << "Starting task " << n << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟耗时任务
    std::cout << "Finished task " << n << std::endl;
}

int main() {
    ThreadPool pool(4); // 创建拥有 4 个工作线程的线程池

    // 向线程池中添加任务
    auto future1 = pool.enqueue(exampleTask, 1);
    auto future2 = pool.enqueue(exampleTask, 2);
    auto future3 = pool.enqueue(exampleTask, 3);
    auto future4 = pool.enqueue(exampleTask, 4);

    // 等待任务完成
    future1.get();
    future2.get();
    future3.get();
    future4.get();

    // 线程池析构时会自动等待所有工作线程完成
    return 0;
}

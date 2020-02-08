#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <stdexcept>

#include <chttp/util/Socket.h>
#include <chttp/Router.h>


class ThreadPool {
private:
    static ThreadPool *instance;
    std::mutex functionMutex;
    std::atomic_bool isPoolRunning = true;
    std::atomic<int> numTasks{};
    std::condition_variable toRun;

    ThreadPool(const std::function<void(Router, std::shared_ptr<Socket>)> &socketFunction, Router router);


    std::queue<std::shared_ptr<Socket>> clientQueue;
    std::vector<std::thread> threads;
    std::function<void(std::shared_ptr<Socket>)> socketHandler;
public:
    static ThreadPool *GetInstance(std::function<void(Router, std::shared_ptr<Socket>)> *socketFunction, Router *);

    void AddWork(const std::shared_ptr<Socket> &);

    void WaitAll();

    ~ThreadPool();
};
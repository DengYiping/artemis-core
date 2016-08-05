#ifndef WORKER_HPP
#define WORKER_HPP
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include "threadtool.h"
namespace thread{
  void thread_main(threadtool::Threadsafe_queue<std::tuple<int, std::vector<std::string>>>& queue,
    std::function<void(int, int, std::string, int)> callback,
    std::vector<std::tuple<std::string, std::string>>& files);
}
#endif

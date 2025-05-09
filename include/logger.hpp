#pragma once
#include <iostream>
#include <mutex>
#include <sstream>

class Logger {
public:
  template <typename... Args> static void log(Args &&...args) {
    std::lock_guard<std::mutex> lock(mutex_);
    (std::cout << ... << args) << std::endl;
  }

private:
  static std::mutex mutex_;
};

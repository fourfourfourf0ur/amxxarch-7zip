#pragma once

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <utility>
#include <vector>

namespace amxxarch {

template <class T>
class ThreadQueue {
public:
  void push(T value) {
    {
      std::lock_guard lock(mutex_);
      queue_.push_back(std::move(value));
    }
    cv_.notify_one();
  }

  std::vector<T> pop_many(size_t max_items) {
    std::vector<T> items;
    items.reserve(max_items);

    std::lock_guard lock(mutex_);
    while (!queue_.empty() && items.size() < max_items) {
      items.push_back(std::move(queue_.front()));
      queue_.pop_front();
    }

    return items;
  }

private:
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<T> queue_;
};

}  // namespace amxxarch

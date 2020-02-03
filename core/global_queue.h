#ifndef GLOBAL_QUEUE_H
#define GLOBAL_QUEUE_H

#include <condition_variable>
#include <mutex>

#include "3rd/LockFreeQueue/lockfree_queue.h"

namespace ActorEngine {
class Actor;
class GlobalQueue  {
 public:
  GlobalQueue() = default;
  ~GlobalQueue() = default;

  static GlobalQueue& Instance() {
    static GlobalQueue global_queue;
    return global_queue;
  }

  void Push(Actor* actor) {
    lfq_.Enqueue(actor);
    cv_.notify_one();
  }

  Actor* Pop() {
    std::unique_lock<std::mutex> lk(mu_);
    cv_.wait(lk, [this] { return lfq_.size() > 0; });
    lk.unlock();

    Actor* actor = nullptr;
    lfq_.Dequeue(actor);
    return actor;
  }

 private:
  std::mutex mu_;
  std::condition_variable cv_;
  LockFreeQueue<ActorEngine::Actor*> lfq_;
};
}  // namespace ActorEngine

#endif
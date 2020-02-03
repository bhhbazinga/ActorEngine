#ifndef ACTOR_H
#define ACTOR_H

#include <atomic>

#include "3rd/LockFreeQueue/lockfree_queue.h"
#include "base/ref.h"
#include "core/global_queue.h"
#include "core/message.h"


namespace ActorEngine {
class Actor;

class Actor : public Ref {
 public:
  // Send Message to actor
  void Send(Actor* dst, Message* msg) {
    msg->set_from(this);
    msg->set_to(dst);
    dst->PushMsg(msg);
  };

 protected:
  virtual ~Actor() = default;
  Actor() = default;

 private:
  void PushMsg(Message* msg) {
    mq_.Enqueue(msg);
    TestAndSetInGlobalFlag();
  };

  Message* PopMsg() {
    Message* msg = nullptr;
    mq_.Dequeue(msg);
    return msg;
  };

  size_t mq_size() const { return mq_.size(); };

  void ClearInGlobalFlag() { in_global_queue_.clear(); }

  void TestAndSetInGlobalFlag() {
    if (!in_global_queue_.test_and_set(std::memory_order_acq_rel)) {
      auto& global_queue = GlobalQueue::Instance();
      global_queue.Push(this);
    }
  }

  LockFreeQueue<Message*> mq_;
  std::atomic_flag in_global_queue_;

  friend void Start();
  friend Actor* NewActor();
  friend void DeleteActor(Actor* actor);
};

class UniqueActor : public Actor {
 protected:
  UniqueActor(const std::string& name) : Actor(), name_(name) {}
  ~UniqueActor() override {}

 private:
  std::string name_;

  friend class Actor;
  friend Actor* NewUniqueActor(const std::string& name);
};

Actor* NewActor();
Actor* NewUniqueActor(const std::string& name);

void DeleteActor(Actor* actor);

// start engine
void Start();

}  // namespace ActorEngine
#endif  // ACTOR_H
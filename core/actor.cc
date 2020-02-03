#include <condition_variable>
#include <thread>
#include <vector>

#include "base/log.h"
#include "core/actor.h"

namespace ActorEngine {
Actor* NewActor() { return new Actor(); }
Actor* NewUniqueActor(const std::string& name) { return new UniqueActor(name); }

void DeleteActor(Actor* actor) { actor->Release(); }

std::condition_variable cond;

void Start() {
  std::vector<std::thread> threads;
  int thread_num = std::thread::hardware_concurrency();
  LOG("start %d threads>>>>>>>>>>>\n", thread_num);

  for (int i = 0; i < thread_num; ++i) {
    threads.emplace_back([] {
      auto& global_queue = GlobalQueue::Instance();
      for (;;) {
        // 1.Pop actor from global queue.
        Actor* actor = global_queue.Pop();
        if (actor == nullptr) {
          std::this_thread::yield();
          continue;
        }

        // 2. Pop message from message queue of actor,
        // only handle one message at a time.
        Message* msg = actor->PopMsg();
        if (msg != nullptr) {
          msg->Handle(actor);
          msg->Release();
        }

        // 3. Clear in_global_mq flag,
        // so another threads can push actor to
        // global queue.
        actor->ClearInGlobalFlag();

        // 4. If the message queue is not empty, make sure the
        // actor is in the global queue.
        if (actor->mq_size() > 0) {
          actor->TestAndSetInGlobalFlag();
        }
      }
    });
  }

  for (auto& t : threads) {
    t.join();
  }
}

}  // namespace ActorEngine
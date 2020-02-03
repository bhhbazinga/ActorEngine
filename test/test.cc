#include <chrono>
#include <cmath>
#include <string>
#include <vector>

#include "base/log.h"
#include "core/actor.h"

ActorEngine::Actor* actor1;
ActorEngine::Actor* actor2;

class TestMsg : public ActorEngine::Message {
 public:
  TestMsg(const std::string& msg) : msg_(msg) {}
  ~TestMsg() override = default;

  void Handle(ActorEngine::Actor* actor) override {
    LOG("%p->%p %s\n", static_cast<void*>(get_from()),
        static_cast<void*>(actor), msg_.c_str());
  }

 private:
  std::string msg_;
};

int main(int argc, char const* argv[]) {
  (void)argc;
  (void)argv;

  std::vector<ActorEngine::Actor*> actors;
  for (int i = 0; i < 10; ++i) {
    actors.push_back(ActorEngine::NewActor());
  }

  std::thread t([&] {
    for (;;) {
      for (size_t i = 0; i < actors.size(); ++i) {
        int from_index = rand() % actors.size();
        int to_index = rand() % actors.size();
        ActorEngine::Actor* from = actors[from_index];
        ActorEngine::Actor* to = actors[to_index];
        char buff[50];
        snprintf(buff, 50, "%d->%d:hello", from_index, to_index);
        std::string msg(buff);
        from->Send(to, new TestMsg(msg));
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });

  ActorEngine::Start();
  t.join();

  // ActorEngine::Message* msg = new TestMsg("aaa");
  // // delete msg;
  return 0;
}

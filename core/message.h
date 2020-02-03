#ifndef MESSAGE_H
#define MESSAGE_H

namespace ActorEngine {
class Actor;

class Message : public Ref {
 public:
  Message() = default;
  virtual ~Message() override = default;

  virtual void Handle(Actor* actor) = 0;

  void set_from(Actor* from) { from_ = from; }
  void set_to(Actor* to) { to_ = to; }

  Actor* get_from() const { return from_; }
  Actor* get_to() const { return to_; }

 private:
  Actor* from_;
  Actor* to_;
};
}  // namespace ActorEngine
#endif
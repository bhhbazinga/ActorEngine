#ifndef REF_H
#define REF_H
#include <atomic>
#include <iostream>
#include "base/log.h"

namespace ActorEngine {
class Ref {
 public:
  void Retain() { ref_count_.fetch_add(1, std::memory_order_relaxed); };
  void Release() {
    if (ref_count_.fetch_sub(1, std::memory_order_relaxed) == 1) {
      delete this;
    }
  }

 protected:
  Ref() : ref_count_(0) { Retain(); }
  virtual ~Ref() = default;

 private:
  std::atomic<int> ref_count_;
};
}  // namespace ActorEngine

#endif  // REF_H
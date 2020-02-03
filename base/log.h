#ifndef LOG_H
#define LOG_H

#define LOG(fmt, ...)                                                  \
  do {                                                                 \
    fprintf(stderr, "[thread_id=%0x]" fmt, std::this_thread::get_id(), \
            __VA_ARGS__);                                              \
  } while (0)

#endif  // LOG_H
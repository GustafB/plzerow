#include <x86intrin.h>

namespace plzerow {
void report_assertion_failure(const char *file_name, int linum,
                              const char *function_name, const char *message);

}

#define PLZEROW_ALWAYS_ASSERT(...)                    \
  do {                                                \
    if (__VA_ARGS__) {                                \
    } else {                                          \
      PLZEROW_REPORT_ASSERTION_FAILURE_(__VA_ARGS__); \
      __builtin_trap();                               \
    }                                                 \
  } while (false)

#define PLZEROW_REPORT_ASSERTION_FAILURE_(...) \
  plzerow::report_assertion_failure(__FILE__, __LINE__, __func__, #__VA_ARGS__)

#if defined(PLZEROW_DEBUG) && PLZEROW_DEBUG
#define PLZEROW_ASSERT(...) PLZEROW_ALWAYS_ASSERT(__VA_ARGS__)
#else
#define PLZEROW_ASSERT(...) PLZEROW_NEVER_ASSERT(__VA_ARGS__)
#endif

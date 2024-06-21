#include <cstdio>
#include <plzerow/assert.hpp>

namespace plzerow {
void report_assertion_failure(const char *file_name, int linum,
                              const char *function_name, const char *message) {
  std::fprintf(stderr,
               "%s:%d: internal check failed in %s: %s\n"
               "plzerow crashed.\n",
               file_name, linum, function_name, message);
  std::fflush(stderr);
}
}

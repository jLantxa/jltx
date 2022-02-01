/*
 * MIT License
 *
 * Copyright (c) 2020-2021 Javier Lancha Vázquez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/


#ifndef _JLTX_INCLUDE_DEBUG_HPP_
#define _JLTX_INCLUDE_DEBUG_HPP_

#include <cstdio>

namespace jltx {
namespace debug {

#ifdef DEBUG_LEVEL
static constexpr int defined_level = DEBUG_LEVEL;
#else
static constexpr int defined_level = 0;
#endif

template <typename... Args>
static void log(FILE* file, const char* tag, const char* levelTag,
                const char* fmt, Args... args) {
  fprintf(file, "[%s] %s: ", levelTag, tag);
  fprintf(file, fmt, args...);
  fprintf(file, "\n");
}

template <typename... Args>
static void log(FILE* file, const char* tag, const char* levelTag,
                const char* str) {
  log(file, tag, levelTag, "%s", str);
}

/** \brief Custom logger to configurable output file */
class Logger final {
 private:
  FILE* file;

 public:
  explicit Logger(FILE* log_file) : file(log_file) {}

#define LOG_FUNCTION(name, level, level_tag)              \
  template <typename... Args>                             \
  inline void name(const char* tag, Args... args) const { \
    if constexpr (defined_level >= level) {               \
      log(file, tag, level_tag, args...);                 \
    }                                                     \
  }

  LOG_FUNCTION(e, -2, "ERROR");
  LOG_FUNCTION(w, -1, "WARNING");
  LOG_FUNCTION(i, 0, "INFO");
  LOG_FUNCTION(d, 1, "DEBUG");
  LOG_FUNCTION(v, 2, "VERBOSE");
};

/** Standard Logger to stdout */
const Logger Log(stdout);

}  // namespace debug
}  // namespace jltx

#endif  // _JLTX_INCLUDE_DEBUG_HPP_

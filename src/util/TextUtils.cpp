/*
 * MIT License
 *
 * Copyright (c) 2020-2022 Javier Lancha Vázquez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "util/TextUtils.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace jltx {

namespace TextUtils {

/**
 * \brief Checks if two std::strings are equal in value.
 * \param str1 First string.
 * \param str2 Second string.
 * \return true if str1 and str2 are equal, false otherwise.
 */
bool Equals(const std::string& str1, const std::string& str2) {
  return (str1.compare(str2) == 0);
}

/**
 * \brief Split a string by a delimiter substring
 * \param str String
 * \param delimiter Delimiter
 * \return A vector with the split substrings.
 */
std::vector<std::string> Split(const std::string& str,
                               const std::string& delimiter) {
  std::vector<std::string> str_vec;
  const std::size_t str_size = str.size();
  const std::size_t sep_size = delimiter.size();

  if (str_size > 0 && sep_size > 0) {
    std::size_t pos = 0;
    std::size_t found;
    while (pos < str_size) {
      found = str.find(delimiter, pos);
      if (found == std::string::npos) {
        // Not found -> add remaining substring
        str_vec.push_back(str.substr(pos));
        break;
      } else {
        // Found -> add substring
        str_vec.push_back(str.substr(pos, found - pos));
        pos = found + sep_size;
      }
    }

    // Add an empty string if the last character is a separator
    if (Equals(str.substr(str_size - sep_size), delimiter)) {
      str_vec.push_back("");
    }
  }

  return str_vec;
}

static void ltrim(std::string& str) {
  str.erase(str.begin(),
            std::find_if(str.begin(), str.end(),
                         [](unsigned char ch) { return !std::isspace(ch); }));
}

static void rtrim(std::string& str) {
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            str.end());
}

void Trim(std::string& str) {
  ltrim(str);
  rtrim(str);
}

[[nodiscard]] std::string TrimCopy(std::string str) {
  Trim(str);
  return str;
}

}  // namespace TextUtils
}  // namespace jltx

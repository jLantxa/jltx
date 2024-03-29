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

#ifndef _JLTX_INCLUDE_UTIL_TEXT_UTILS_HPP_
#define _JLTX_INCLUDE_UTIL_TEXT_UTILS_HPP_

#include <string>
#include <vector>

namespace jltx {

/** Utility functions for manipulating text. */
namespace TextUtils {

/** Compare two strings and check whether they are the same.
 *
 * \param str1 First string
 * \param str2 Second string
 * \return true if the contents of both strings are the same
 */
bool Equals(const std::string& str1, const std::string& str2);

/** \brief Split a string into substrings using another string as a delimiter.
 *
 * The substrings are returned in a std:vector<std::string>.
 * <br>
 * Example: Split("127.0.0.1", ".") -> {"127", "0", "0", "1"}
 *
 * \param str String
 * \param str Delimiter string
 * \return std::vector containing the substrings
 */
std::vector<std::string> Split(const std::string& str,
                               const std::string& delimiter);

/**
 * @brief Remove trailing whitespaces in place
 */
void Trim(std::string& str);

/**
 * @brief Return a new string with all trailing whitespaces removed
 */
[[nodiscard]] std::string TrimCopy(std::string str);

}  // namespace TextUtils
}  // namespace jltx

#endif  // _JLTX_INCLUDE_UTIL_TEXT_UTILS_HPP_

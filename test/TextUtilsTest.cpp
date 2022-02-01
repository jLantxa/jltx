/*
 * MIT License
 *
 * Copyright (c) 2022 Javier Lancha Vázquez
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

#include <gtest/gtest.h>

#include "util/TextUtils.hpp"

TEST(TextUtilsTest, Equals) {
  const std::string str_a{"any string"};
  const std::string str_b{"other string"};

  EXPECT_TRUE(jltx::TextUtils::Equals("any string", "any string"));
  EXPECT_TRUE(jltx::TextUtils::Equals("any string", str_a));
  EXPECT_TRUE(jltx::TextUtils::Equals(str_a, "any string"));
  EXPECT_FALSE(jltx::TextUtils::Equals(str_a, str_b));
  EXPECT_FALSE(jltx::TextUtils::Equals("whatever", str_b));
}

TEST(TextUtilsTest, Split) {
  const std::string str{"first;second;third"};
  const std::vector<std::string> substrings = jltx::TextUtils::Split(str, ";");

  EXPECT_EQ(substrings.size(), 3);
  EXPECT_EQ(substrings[0], "first");
  EXPECT_EQ(substrings[1], "second");
  EXPECT_EQ(substrings[2], "third");
}

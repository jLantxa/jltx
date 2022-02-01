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

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

#include <gtest/gtest.h>

#include <algorithm>
#include <array>

#include "containers/RingArray.hpp"

TEST(RingArrayTest, FillAndEmpty) {
  jltx::RingArray<int, 4> ring;

  ASSERT_EQ(ring.Size(), 4);

  EXPECT_TRUE(ring.Empty());
  EXPECT_FALSE(ring.Full());
  EXPECT_EQ(ring.FillLevel(), 0);

  ASSERT_DEATH(ring.Pop(), "");
  ASSERT_DEATH(ring.Back(), "");
  ASSERT_DEATH(ring.Front(), "");
  for (std::size_t i = 0; i < ring.Size(); ++i) {
    ASSERT_DEATH(ring[i], "");
  }

  ring.Push(0);
  EXPECT_FALSE(ring.Empty());
  EXPECT_FALSE(ring.Full());
  EXPECT_EQ(ring.FillLevel(), 1);

  ring.Push(1);
  EXPECT_FALSE(ring.Full());
  EXPECT_EQ(ring.FillLevel(), 2);

  ring.Push(2);
  EXPECT_FALSE(ring.Full());
  EXPECT_EQ(ring.FillLevel(), 3);

  ring.Push(3);
  EXPECT_TRUE(ring.Full());
  EXPECT_EQ(ring.FillLevel(), 4);

  for (std::size_t i = 0; i < ring.Size(); ++i) {
    EXPECT_EQ(ring[i], i);
  }

  for (std::size_t i = 0; i < ring.Size(); ++i) {
    int element = ring.Pop();
    EXPECT_EQ(element, i);
    EXPECT_EQ(ring.FillLevel(), 4 - (i + 1));
  }
  ASSERT_TRUE(ring.Empty());
}

TEST(RingArrayTest, InitializerList) {
  jltx::RingArray<int, 4> ring = {0, 1, 2};

  ASSERT_EQ(ring.FillLevel(), 3);
  for (std::size_t i = 0; i < ring.FillLevel(); ++i) {
    EXPECT_EQ(ring[i], i);
  }
}

TEST(RingArrayTest, Overwrite) {
  jltx::RingArray<int, 4> ring = {0, 1, 2, 3};
  ASSERT_EQ(ring.FillLevel(), 4);
  ASSERT_TRUE(ring.Full());

  ring.Push(4);
  ring.Push(5);
  EXPECT_TRUE(ring.Full());
  for (std::size_t i = 0; i < ring.FillLevel(); ++i) {
    EXPECT_EQ(ring[i], i + 2);
  }
}

TEST(RingArrayTest, Iterator) {
  jltx::RingArray<int, 4> ring = {0, 1, 2};

  std::vector<int> vec(3);
  for (auto& num : ring) {
    vec.push_back(num);
  }

  for (int i = 0; i < 3; ++i) {
    vec[i] = i;
  }
}

TEST(RingArrayTest, IteratorEmpty) {
  jltx::RingArray<int, 4> ring;

  for (auto& num : ring) {
    (void)num;
    FAIL();
  }
}

TEST(RingArrayTest, IteratorWrapAround) {
  jltx::RingArray<int, 4> ring = {0, 1, 2, 3};
  ring.Push(4);

  std::vector<int> vec;
  for (auto& num : ring) {
    vec.push_back(num);
  }

  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(vec[i], (i + 1));
  }
}

TEST(RingArrayTest, Structs) {
  struct _Object {
    int a;
    int b;

    bool operator==(const _Object& other) const {
      return ((a == other.a) && (b == other.b));
    }
  };

  jltx::RingArray<_Object, 4> ring = {{11, 12}, {21, 22}, {31, 32}, {41, 42}};

  ASSERT_TRUE(ring.Full());
  EXPECT_EQ(ring[0], (_Object{11, 12}));
  EXPECT_EQ(ring.Pop(), (_Object{11, 12}));
  EXPECT_EQ(ring[0], (_Object{21, 22}));
  EXPECT_EQ(ring.FillLevel(), 3);
}

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

#ifndef _JLTX_INCLUDE_CONTAINERS_RING_ARRAY_HPP_
#define _JLTX_INCLUDE_CONTAINERS_RING_ARRAY_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>

namespace jltx {

template <typename T, std::size_t size>
class RingArray {
 public:
  class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    Iterator(RingArray& ring, std::size_t index)
        : m_ring(ring), m_index(index) {}

    bool operator==(const Iterator& other) {
      return (m_ring.m_array == other.m_ring.m_array) &&
             (m_ring.m_head == other.m_ring.m_head) &&
             (m_ring.m_tail == other.m_ring.m_tail) &&
             (m_index == other.m_index);
    }

    bool operator!=(const Iterator& other) { return !(*this == other); }

    Iterator& operator++() {
      m_index++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    reference operator*() const { return m_ring[m_index]; }

    pointer operator->() const { return &m_ring[m_index]; }

   private:
    RingArray& m_ring;
    std::size_t m_index;
  };

  RingArray() = default;

  RingArray(std::initializer_list<T> list) {
    const std::size_t list_size = list.size();
    assert(size >= list_size);

    const auto& list_begin = list.begin();
    for (std::size_t i = 0; i < list_size; ++i) {
      m_array[i] = *(list_begin + i);
    }

    m_fill_level = list_size;
    m_head = 0;
    m_tail = list_size - 1;
  }

  T& Back() {
    assert(m_fill_level > 0);
    return m_array[m_tail];
  }

  const T& Back() const { return Back(); }

  T& Front() {
    assert(m_fill_level > 0);
    return m_array[m_head];
  }

  const T& Front() const { return Front(); }

  T& operator[](std::size_t i) {
    assert(i < m_fill_level);

    const std::size_t n = Index(m_head + i);
    return m_array[n];
  }

  const T& operator[](std::size_t i) const { return *this[i]; }

  void Push(T element) {
    m_tail = Index(m_tail + 1);
    m_array[m_tail] = element;

    if (Full()) {
      m_head = Index(m_head + 1);
    }
    m_fill_level = std::min(size, m_fill_level + 1);
  }

  T Pop() {
    assert(m_fill_level > 0);

    m_fill_level = std::max(0UL, m_fill_level - 1);
    T head = m_array[m_head];
    m_head = Index(m_head + 1);
    return head;
  }

  constexpr std::size_t Size() const { return size; }

  std::size_t FillLevel() const { return m_fill_level; }

  bool Empty() const { return (m_fill_level == 0); }

  bool Full() const { return (m_fill_level >= size); }

  std::size_t m_head = 0;
  std::size_t m_tail = size - 1;

  [[nodiscard]] Iterator begin() { return Iterator(*this, 0); }

  [[nodiscard]] Iterator end() { return Iterator(*this, m_fill_level); }

 private:
  T m_array[size];
  std::size_t m_fill_level = 0;

  constexpr std::size_t Index(std::size_t i) const { return (i % size); }

  friend class Iterator;
};

}  // namespace jltx

#endif  // _JLTX_INCLUDE_CONTAINERS_RING_ARRAY_HPP_

/*
 * MIT License
 *
 * Copyright (c) 2020 Javier Lancha Vázquez
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

#include <cmath>

namespace jltx {
namespace math {

/**
 * @brief Wrap radians to a [-PI, PI] scale
 *
 * @tparam T Number type
 * @param x Radians
 */
template <typename T>
T wrap_radians(T x) {
  if (x > 0) {
    return static_cast<T>(fmod(x + M_PI, 2 * M_PI)) - static_cast<T>(M_PI);
  } else {
    return static_cast<T>(fmod(x - M_PI, 2 * M_PI)) + static_cast<T>(M_PI);
  }
}

/**
 * @brief Wrap degrees to a [0, 360) scale
 *
 * @tparam T Number type
 * @param x Degrees
 */
template <typename T>
T wrap_degrees(T deg) {
  return fmod(deg, 360);
}

/**
 * @brief Sine function via an 11th degree Taylor series
 *
 * @tparam T Type
 */
template <typename T>
T sin11(T x) {
  x = wrap_radians<T>(x);

  static constexpr uint32_t f3 = 6;
  static constexpr uint32_t f5 = f3 * 4 * 5;
  static constexpr uint32_t f7 = f5 * 6 * 7;
  static constexpr uint32_t f9 = f7 * 8 * 9;
  static constexpr uint32_t f11 = f9 * 10 * 11;

  const T x2 = x * x;
  const T x3 = x * x2;
  const T x5 = x3 * x2;
  const T x7 = x5 * x2;
  const T x9 = x7 * x2;
  const T x11 = x9 * x2;
  return (x - (x3 / f3) + (x5 / f5) - (x7 / f7) + (x9 / f9) - (x11 / f11));
}

/**
 * @brief Cosine function via an 11th degree Taylor series of the sine function
 *
 * @tparam T Type
 */
template <typename T>
T cos11(T x) {
  return sin11(x + static_cast<T>(M_PI) / static_cast<T>(2));
}

}  // namespace math
}  // namespace jltx

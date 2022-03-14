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
#include <cmath>
#include <math/math.hpp>
#include <numeric>
#include <vector>

TEST(MathTest, sin11) {
  // Test precision with float
  static const float step = 0.0001f;
  std::vector<float> errors;

  float x = static_cast<float>(-2 * M_PI);
  while (x <= 2 * M_PI) {
    const float y_sin = sinf(x);
    const float y_app = jltx::math::sin11(x);
    const float error = abs(y_app - y_sin);
    errors.push_back(error);
    x += step;
  }

  const float mean = std::accumulate(errors.begin(), errors.end(), 0.0f) /
                     static_cast<float>(errors.size());
  const float max = *std::max_element(errors.begin(), errors.end());
  ASSERT_LE(max, 5e-4);   // Max error is bound to 5e-4
  ASSERT_LE(mean, 5e-5);  // Mean error is bound to 5e-5
}

TEST(MathTest, cos11) {
  // Test precision with float
  static const float step = 0.0001f;
  std::vector<float> errors;

  float x = static_cast<float>(-2 * M_PI);
  while (x <= 2 * M_PI) {
    const float y_sin = cosf(x);
    const float y_app = jltx::math::cos11(x);
    const float error = abs(y_app - y_sin);
    errors.push_back(error);
    x += step;
  }

  const float mean = std::accumulate(errors.begin(), errors.end(), 0.0f) /
                     static_cast<float>(errors.size());
  const float max = *std::max_element(errors.begin(), errors.end());
  ASSERT_LE(max, 5e-4);   // Max error is bound to 5e-4
  ASSERT_LE(mean, 5e-5);  // Mean error is bound to 5e-5
}

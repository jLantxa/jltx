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

#ifndef _JLTX_INCLUDE_OPTIMIZATION_GRADIENT_HPP_
#define _JLTX_INCLUDE_OPTIMIZATION_GRADIENT_HPP_

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace jltx {
namespace opt {

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

template <typename T, uint32_t nparams>
struct OptimizationProblem {
  virtual std::vector<T> Estimate(const std::vector<T>& x,
                                  const std::array<T, nparams>& w) = 0;

  virtual T Error(const std::vector<T>& x, const std::array<T, nparams>& w,
                  const std::vector<T>& y) = 0;

  virtual std::array<T, nparams> Gradient(const std::vector<T>& x,
                                          const std::array<T, nparams>& w,
                                          const std::vector<T>& y) = 0;
};

template <typename T, uint32_t nparams>
class GradientSolver {
 public:
  GradientSolver(OptimizationProblem<T, nparams>* problem, T alpha)
      : m_problem(problem), m_alpha(alpha) {
    std::srand(std::time(nullptr));
  }

  std::array<T, nparams> Solve(uint32_t iterations, const std::vector<T>& x,
                               const std::vector<T>& y) {
    std::array<T, nparams> w = GetRandomInitParams();

    for (uint32_t i = 0; i < iterations; ++i) {
      auto grad = m_problem->Gradient(x, w, y);

      // // Optionally print error
      // const T error = problem->Error(x, w, y);
      // std::cout <<
      //     (i+1) << "/" << iterations << ": " <<
      //     "Error: " << error << std::endl;

      // Update gradients
      for (uint32_t i = 0; i < nparams; ++i) {
        w[i] -= m_alpha * grad[i];
      }
    }

    return w;
  }

 private:
  OptimizationProblem<T, nparams>* m_problem;

  T m_alpha;

  std::array<T, nparams> GetRandomInitParams() {
    std::array<T, nparams> params;
    for (T& w : params) {
      w = std::rand();
    }
    return params;
  }
};

template <typename T, uint32_t nparams>
struct PolynomicFitProblem : public OptimizationProblem<T, nparams> {
  std::vector<T> Estimate(const std::vector<T>& x,
                          const std::array<T, nparams>& w) override {
    std::vector<T> estimates;

    for (uint32_t n = 0; n < x.size(); ++n) {
      T estimate = 0;
      for (uint32_t i = 0; i < nparams; ++i) {
        estimate += w[i] * pow(x[n], i);
      }
      estimates.push_back(estimate);
    }

    return estimates;
  }

  T Error(const std::vector<T>& x, const std::array<T, nparams>& w,
          const std::vector<T>& y) override {
    auto estimates = Estimate(x, w);
    T error = 0;
    for (uint32_t i = 0; i < x.size(); ++i) {
      error += pow((y[i] - x[i]), 2) / x.size();
    }
    return error;
  }

  std::array<T, nparams> Gradient(const std::vector<T>& x,
                                  const std::array<T, nparams>& w,
                                  const std::vector<T>& y) override {
    std::array<T, nparams> gradient;

    std::vector<T> partial_terms;
    std::vector<T> estimates = Estimate(x, w);
    for (uint32_t n = 0; n < x.size(); ++n) {
      partial_terms.push_back(-2 * (y[n] - estimates[n]));
    }

    for (uint32_t i = 0; i < nparams; ++i) {
      T grad = 0;
      for (uint32_t n = 0; n < x.size(); ++n) {
        grad += partial_terms[n] * pow(x[i], n);
      }
      gradient[i] = grad;
    }

    return gradient;
  }
};

}  // namespace opt
}  // namespace jltx

#endif  // _JLTX_INCLUDE_OPTIMIZATION_GRADIENT_HPP_

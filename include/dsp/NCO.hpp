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

#ifndef _JLTX_INCLUDE_DSP_NCO_HPP_
#define _JLTX_INCLUDE_DSP_NCO_HPP_

#include <array>
#include <cmath>
#include <cstdint>

namespace jltx {
namespace dsp {

template <uint8_t bit_depth>
class SineLUT {
 public:
  SineLUT() {
    for (uint32_t k = 0; k < m_length; k++) {
      m_table[k] = sinf(static_cast<float>(2 * M_PI * k) / m_length);
    }
  }

  [[nodiscard]] float operator[](uint32_t i) const {
    return m_table[i & m_mask];
  }

  static constexpr float ROTATION = 2.0f * (1u << (8 * sizeof(uint32_t) - 1));

 private:
  static constexpr uint32_t m_length = 1 << bit_depth;
  static constexpr uint32_t m_mask = m_length - 1;
  std::array<float, m_length> m_table;
};

template <uint8_t bit_depth>
class NCO {
 public:
  NCO(float freq, float sample_rate, const SineLUT<bit_depth>& table)
      : m_table(table),
        m_phase(0),
        m_frequency(freq),
        m_sample_rate(sample_rate) {
    ResetPhaseDelta();
  }

  [[nodiscard]] float operator()() {
    const unsigned int index = m_phase >> (sizeof(uint32_t) * 8 - bit_depth);
    float value = m_table[index];
    m_phase += m_delta_phase;
    return value;
  }

  [[nodiscard]] float operator[](uint32_t i) const { return m_table[i]; }

  [[nodiscard]] float Frequency() const;
  void SetFrequency(float frequency) { m_frequency = frequency; }

  [[nodiscard]] float SampleRate() const;
  void SetSampleRate(float sample_sate) { m_sample_rate = sample_sate; };

  void ResetPhase() { m_phase = 0; }

 private:
  const SineLUT<bit_depth>& m_table;

  uint32_t m_phase;
  uint32_t m_delta_phase;

  float m_frequency;
  float m_sample_rate;

  void ResetPhaseDelta() {
    m_delta_phase =
        (uint32_t)(m_frequency * SineLUT<bit_depth>::ROTATION / m_sample_rate);
  }
};

}  // namespace dsp
}  // namespace jltx

#endif  // _JLTX_INCLUDE_DSP_NCO_HPP_

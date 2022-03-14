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

#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "audio/AlsaAudioSink.hpp"
#include "dsp/NCO.hpp"

static constexpr uint8_t BIT_DEPTH = 10;
static constexpr uint32_t BUFFER_SIZE = 256;

int main(int argc, char* argv[]) {
  const uint32_t freq = atoi(argv[1]);
  const uint32_t sample_rate = atoi(argv[2]);
  const float length = static_cast<float>(atof(argv[3]));

  jltx::dsp::SineLUT<BIT_DEPTH> lut;
  jltx::dsp::NCO<BIT_DEPTH> sin_nco(static_cast<float>(freq),
                            static_cast<float>(sample_rate), lut);

  jltx::audio::AlsaAudioSink audio_sink(sample_rate);

  int32_t remaining_samples = sample_rate * static_cast<uint32_t>(length);
  float buffer[BUFFER_SIZE];

  while (remaining_samples > 0) {
    const int32_t write_size =
        std::min(BUFFER_SIZE, static_cast<uint32_t>(remaining_samples));
    for (int32_t i = 0; i < write_size; ++i) {
      buffer[i] = sin_nco();
    }
    audio_sink.Send(buffer, write_size);
    remaining_samples -= write_size;
  };

  return 0;
}

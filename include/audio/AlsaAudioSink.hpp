/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Javier Lancha Vázquez
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

#ifndef _JLTX_INCLUDE_AUDIO_ALSA_AUDIO_SINK_HPP_
#define _JLTX_INCLUDE_AUDIO_ALSA_AUDIO_SINK_HPP_

#include <alsa/asoundlib.h>

#include <cstdint>

namespace jltx {
namespace audio {

class AlsaAudioSink {
 public:
  AlsaAudioSink(uint32_t sample_rate);
  ~AlsaAudioSink();

  uint32_t Send(float* buffer, uint32_t size);

  [[nodiscard]] uint32_t SampleRate() const;

 private:
  snd_pcm_t* m_pcm_handle;
  uint32_t m_sample_rate;
};

}  // namespace audio
}  // namespace jltx

#endif  // _JLTX_INCLUDE_AUDIO_ALSA_AUDIO_SINK_HPP_

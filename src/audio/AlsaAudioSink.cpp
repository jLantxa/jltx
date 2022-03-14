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

#include "audio/AlsaAudioSink.hpp"

#define PCM_DEVICE "default"
#define CHANNELS 1

namespace jltx {
namespace audio {

AlsaAudioSink::AlsaAudioSink(uint32_t sample_rate) {
  snd_pcm_hw_params_t* hw_params;

  snd_pcm_open(&m_pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);

  snd_pcm_hw_params_malloc(&hw_params);
  snd_pcm_hw_params_any(m_pcm_handle, hw_params);

  snd_pcm_hw_params_set_access(m_pcm_handle, hw_params,
                               SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_format(m_pcm_handle, hw_params, SND_PCM_FORMAT_FLOAT);
  snd_pcm_hw_params_set_channels(m_pcm_handle, hw_params, CHANNELS);
  snd_pcm_hw_params_set_rate_near(m_pcm_handle, hw_params, &sample_rate, 0);
  snd_pcm_hw_params(m_pcm_handle, hw_params);

  unsigned int reported_sample_rate;
  snd_pcm_hw_params_get_rate(hw_params, &reported_sample_rate, 0);
  m_sample_rate = reported_sample_rate;

  snd_pcm_hw_params_free(hw_params);
  snd_pcm_prepare(m_pcm_handle);
}

AlsaAudioSink::~AlsaAudioSink() {
  snd_pcm_drain(m_pcm_handle);
  snd_pcm_close(m_pcm_handle);
}

uint32_t AlsaAudioSink::Send(float* buffer, uint32_t size) {
  if (size == 0) {  // Nothing to write
    return 0;
  }

  snd_pcm_sframes_t ret = snd_pcm_writei(m_pcm_handle, buffer, size);
  if (ret > 0) {
    return static_cast<uint32_t>(ret);
  } else {
    if (ret == -EPIPE) {
      snd_pcm_prepare(m_pcm_handle);
    }
    return 0;
  }
}

[[nodiscard]] uint32_t AlsaAudioSink::SampleRate() const {
  return m_sample_rate;
}

}  // namespace audio
}  // namespace jltx

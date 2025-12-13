/**
 * @brief Sound library
 */

#ifndef _SOUND_H
#define _SOUND_H

#ifndef _WIN64

#include <driver/i2s.h>

#define I2S_DOUT 17  // Data Out
#define I2S_BCLK 0   // Bit Clock
#define I2S_LRC 18   // Word Select (WS) - Left/Right Clock (LRCK)
#define I2S_PORT I2S_NUM_0

#define CHUNK_SIZE 20000

void SetupI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, nullptr);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
}

int16_t* GenerateSound(const uint8_t* sample, uint32_t size) {
  int16_t* output = (int16_t*)heap_caps_malloc(sizeof(int16_t) * size * 2, MALLOC_CAP_SPIRAM);

  for (uint32_t i = 0; i < size; i++) {
    output[2 * i] = (int16_t)sample[i] * 120;  // Left chanel
    output[2 * i + 1] = 0;                     // Right chanel
  }

  return output;
}

void PlaySound(const uint8_t* sample, uint32_t size) {
  auto output = GenerateSound(sample, size);

  size_t bytes_written;
  i2s_write(I2S_PORT, output, size * 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);

  free(output);

  i2s_zero_dma_buffer(I2S_PORT);
}

void PlayMusic(const uint8_t* sample, uint32_t size) {
  uint32_t start_index = 0;

  while (start_index < size) {
    uint32_t current_chunk_size = CHUNK_SIZE;

    if (start_index + CHUNK_SIZE > size) {
      current_chunk_size = size - start_index;
    }

    uint32_t end_index = start_index + current_chunk_size;
    auto output = GenerateSound(&sample[start_index], end_index - start_index + 1);
    size_t bytes_written;

    i2s_write(I2S_PORT, output, (end_index - start_index + 1) * 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);

    free(output);

    start_index += current_chunk_size;
  }

  i2s_zero_dma_buffer(I2S_PORT);
}

#endif
#endif  // _SOUND_H

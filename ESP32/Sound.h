/**
 * @brief Sound library
 */

#ifndef _SOUND_H
#define _SOUND_H

#ifndef _WIN64

#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

#define I2S_DOUT 17  // Data Out
#define I2S_BCLK 0   // Bit Clock
#define I2S_LRC 18   // Word Select (WS) - Left/Right Clock (LRCK)
#define I2S_PORT I2S_NUM_0

#define CHUNK_SIZE 20000

Audio audio;
SPIClass hSPI(HSPI);

uint16_t* GenerateSound(const uint8_t* sample, uint32_t size) {
  uint16_t* output = (uint16_t*)heap_caps_malloc(sizeof(uint16_t) * size * 2, MALLOC_CAP_SPIRAM);

  for (uint32_t i = 0; i < size; i++) {
    output[2 * i] = (uint16_t)sample[i] * 128;      // Left chanel
    output[2 * i + 1] = (uint16_t)sample[i] * 128;  // Right chaneld
  }

  return output;
}

void PlaySoundFromSample(const uint8_t* sample, uint32_t size) {
  auto output = GenerateSound(sample, size);

  i2s_channel_write(audio.getI2SHandle(), output, size * 2 * sizeof(uint16_t), nullptr, portMAX_DELAY);
  audio.zeroI2Sbuff();

  free(output);
}

void PlayMusicFromSample(const uint8_t* sample, uint32_t size) {
  struct sound_data_t {
    uint8_t* sample;
    uint32_t size;
  };

  sound_data_t* data = (sound_data_t*)malloc(sizeof(sound_data_t));
  data->sample = const_cast<uint8_t*>(sample);
  data->size = size;

  xTaskCreatePinnedToCore(
    [](void* pvParameters) {
      sound_data_t* data = (sound_data_t*)pvParameters;
      uint32_t start_index = 0;

      while (start_index < data->size) {
        uint32_t current_chunk_size = CHUNK_SIZE;

        if (start_index + CHUNK_SIZE > data->size) {
          current_chunk_size = data->size - start_index;
        }

        uint32_t end_index = start_index + current_chunk_size;
        auto output = GenerateSound(&data->sample[start_index], end_index - start_index + 1);

        i2s_channel_write(audio.getI2SHandle(), output, (end_index - start_index + 1) * 2 * sizeof(uint16_t), nullptr, portMAX_DELAY);
        free(output);

        start_index += current_chunk_size;
      }

      audio.zeroI2Sbuff();
      free(data);
      vTaskDelete(NULL);
    },
    __FUNCTION__, 1024 * 10, data, 1, NULL, 1);
}

void PrintAudioInfo(Audio::msg_t m) {
  // Only print important messages to reduce Serial clutter
  Serial.printf("%s: %s\n", m.s, m.msg);

  if (!strcmp(m.s, "eof")) {
    MusicState.SetValue(MUSIC_STATE_TYPE::MSG_EOF);
  }
}

bool CheckMusicFile(String filename) {
  filename.toLowerCase();
  if (filename.endsWith(".aac")) return true;
  if (filename.endsWith(".flac")) return true;
  if (filename.endsWith(".mp3")) return true;
  if (filename.endsWith(".ogg")) return true;
  if (filename.endsWith(".wav")) return true;
  if (filename.endsWith(".m4a")) return true;
  return false;
}

std::vector<std::string> GetListMusicFiles(fs::FS& fs, const char* dirname, uint8_t levels) {
  std::vector<std::string> playlist;

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return {};
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return {};
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      if (levels) {
        // Recursive search for subdirectories (optional)
        // listMusicFiles(fs, file.path(), levels - 1);
      }
    } else {
      String filename = String(file.name());
      // Filter out hidden files (starting with .) and check audio extension
      if (!filename.startsWith(".") && CheckMusicFile(filename)) {
        // Standardize path as the SD library may return names with or without leading '/'
        if (!filename.startsWith("/")) {
          filename = "/" + filename;
        }

        playlist.push_back(filename.c_str());
      }
    }
    file = root.openNextFile();

    vTaskDelay(1);
  }
  root.close();

  std::sort(playlist.begin(), playlist.end());

  return playlist;
}

std::vector<std::string> ExportListMusicFiles() {
  return GetListMusicFiles(SD, "/", 0);  // Scan the root directory
}

void PlayTrack(const char* name) {
  audio.connecttoFS(SD, name);
}

void SetupSound() {
  hSPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

  if (!SD.begin(SD_CS)) {
    Serial.println("Error: SD Card not found!");
    while (true)
      ;  // Force reboot
  }

  Audio::audio_info_callback = PrintAudioInfo;

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.forceMono(true);
  audio.setVolume(DEFAULT_VOLUME);
  audio.setAudioTaskCore(1);

  PlayTrack("/sound/null.mp3");
}

void RunMusicTask() {
  audio.loop();
}

#endif
#endif  // _SOUND_H

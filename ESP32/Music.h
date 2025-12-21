/**
 * @brief Music library
 */

#ifndef _MUSIC_H
#define _MUSIC_H

#ifndef _WIN64

#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12

namespace music {
Audio *audio;

static void PrintAudioInfo(Audio::msg_t m) {
  // Only print important messages to reduce Serial clutter
  Serial.printf("%s: %s\n", m.s, m.msg);

  if (!strcmp(m.s, "eof")) {
    MusicState.SetValue(MUSIC_STATE_TYPE::MSG_EOF);
  }
}

static bool CheckMusicFile(String filename) {
  filename.toLowerCase();
  if (filename.endsWith(".aac")) return true;
  if (filename.endsWith(".flac")) return true;
  if (filename.endsWith(".mp3")) return true;
  if (filename.endsWith(".ogg")) return true;
  if (filename.endsWith(".wav")) return true;
  if (filename.endsWith(".m4a")) return true;
  return false;
}

static std::vector<std::string> GetListMusicFiles(fs::FS &fs, const char *dirname, uint8_t levels) {
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

void PlayTrack(const char *name) {
  audio->connecttoFS(SD, name);
}

void Setup() {
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  if (!SD.begin(SD_CS)) {
    Serial.println("Error: SD Card not found!");
    while (true)
      ;  // Force reboot
  }

  Audio::audio_info_callback = PrintAudioInfo;
  audio = new Audio();

  audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio->forceMono(true);
  audio->setAudioTaskCore(1);
}

void Uninstall() {
  delete audio;
  audio = nullptr;
}

void RunMusicTask() {
  if (audio) {
    audio->loop();
  }
}
}
#endif
#endif  // _MUSIC_H
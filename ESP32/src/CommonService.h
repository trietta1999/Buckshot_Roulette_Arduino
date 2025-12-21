/**
 * @brief Common service library
 */

#ifndef _COMMON_SERVICE_H
#define _COMMON_SERVICE_H

#include <cstdint>
#include "CommonDataType.h"
#include "ArduinoJson-v7.4.1.h"

void InitData();
void CommonServiceProcess();
JsonDocument ProcessRequest(HWND, uint32_t, JsonDocument);
JsonDocument CommonSendRequest(uint32_t);
JsonDocument CommonSendRequestWithData(uint32_t, JsonDocument);
void CommonBeep(uint16_t, uint16_t);

void PlaySoundWrapper(SOUND_TYPE type);
std::vector<std::string> ExportListMusicFilesWrapper();
void PlayMusicWrapper(std::string name);
void SetMusicStateWrapper(MUSIC_STATE_TYPE state);
uint8_t GetMusicMaxVolumeWrapper();
void SetMusicVolumeWrapper(uint8_t vol);
uint8_t GetCurrentMusicPercentWrapper();

#endif

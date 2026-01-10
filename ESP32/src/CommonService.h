/**
 * @brief Common service library
 */

#ifndef _COMMON_SERVICE_H
#define _COMMON_SERVICE_H

#include "CommonDataType.h"

void InitData();
void CommonServiceProcess();

void PlaySoundWrapper(SOUND_TYPE type);
std::vector<std::string> ExportListMusicFilesWrapper();
void PlayMusicWrapper(std::string name);
void SetMusicStateWrapper(MUSIC_STATE_TYPE state);
uint8_t GetMusicMaxVolumeWrapper();
void SetMusicVolumeWrapper(uint8_t vol);
uint8_t GetCurrentMusicPercentWrapper();

#endif

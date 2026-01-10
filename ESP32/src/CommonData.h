/**
 * @brief Common data
 */

#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

#ifdef _WIN64
namespace debug_data
{
    extern CData<std::vector<std::string>> InputParamList;
    extern CData<PLAYER_TYPE> CurrentPlayer;
    extern CData<ITEM_TYPE> CurrentItemType;
    extern CData<std::tuple<PLAYER_TYPE, uint8_t, uint8_t>> PlayerHP;
    extern CData<std::string> SpecialCommand;
}
#endif

extern CData<uint8_t> Brightness;
extern CData<STATE_TYPE> CurrentState;
extern CData<bool> GuiBlockState;
extern CData<bool> SoundEnable;
extern CData<bool> MusicEnable;
extern CData<uint32_t> CurrentMusicIndex;
extern CData<uint8_t> CurrentMusicPercent;
extern CData<MUSIC_STATE_TYPE> MusicState;
extern CData<std::vector<std::string>> Playlist;

void UpdateAll();

#endif // !_COMMON_DATA_H

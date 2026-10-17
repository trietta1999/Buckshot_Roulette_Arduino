/**
 * @brief Common data definitions and external declarations
 */

#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

#ifdef _WIN64
namespace debug_data
{
    extern CData<std::vector<std::string>> InputParamList; // Debug console input parameters
    extern CData<PLAYER_TYPE> CurrentPlayer; // Current player in debug mode
    extern CData<ITEM_TYPE> CurrentItemType; // Current item type for testing
    extern CData<std::tuple<PLAYER_TYPE, uint8_t, uint8_t>> PlayerHP; // Player HP override data
    extern CData<std::string> SpecialCommand; // Special debug commands
}
#endif

extern CData<uint8_t> Brightness; // Display brightness level (0-255)
extern CData<STATE_TYPE> CurrentState; // Current game state
extern CData<bool> GuiBlockState; // Global GUI interaction lock state
extern CData<bool> SoundEnable; // SFX toggle state
extern CData<bool> MusicEnable; // Music toggle state
extern CData<uint32_t> CurrentMusicIndex; // Index of the playing track in playlist
extern CData<uint8_t> CurrentMusicPercent; // Playback progress percentage
extern CData<MUSIC_STATE_TYPE> MusicState; // Current music playback state
extern CData<std::vector<std::string>> Playlist; // List of available music filenames

void UpdateAll(); // Function to refresh all application logic

#endif // !_COMMON_DATA_H

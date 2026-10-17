/**
 * @brief Common data instances and state management
 */

#include "CommonData.h"

#ifdef _WIN64
namespace debug_data
{
    CData<std::vector<std::string>> InputParamList; // Buffers debug console input
    CData<PLAYER_TYPE> CurrentPlayer; // Overrides current player
    CData<ITEM_TYPE> CurrentItemType; // Injects specific items for testing
    CData<std::tuple<PLAYER_TYPE, uint8_t, uint8_t>> PlayerHP; // Overrides player health (player, level, hp)
    CData<std::string> SpecialCommand; // Catch-all for miscellaneous debug signals
}
#endif

CData<uint8_t> Brightness; // Current screen brightness (0-255)
CData<STATE_TYPE> CurrentState; // Global game state
CData<bool> GuiBlockState; // Prevents UI interactions when true
CData<bool> SoundEnable; // Global SFX switch
CData<bool> MusicEnable; // Global music switch
CData<uint32_t> CurrentMusicIndex; // ID of current track in playlist
CData<uint8_t> CurrentMusicPercent; // Progress of current track (0-100)
CData<MUSIC_STATE_TYPE> MusicState; // Playing, Paused, Stopped, etc.
CData<std::vector<std::string>> Playlist; // List of music file names

// Resets the 'changed' flag for all tracked data to prevent re-triggering logic
void UpdateAll()
{
    Brightness.ResetState();
    CurrentState.ResetState();
    GuiBlockState.ResetState();
    MusicState.ResetState();
    CurrentMusicIndex.ResetState();
    CurrentMusicPercent.ResetState();
#ifdef _WIN64
    debug_data::CurrentPlayer.ResetState();
    debug_data::CurrentItemType.ResetState();
    debug_data::PlayerHP.ResetState();
    debug_data::SpecialCommand.ResetState();
#endif
}

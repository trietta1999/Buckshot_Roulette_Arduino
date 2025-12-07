/**
 * @brief Common data
 */

#include "CommonData.h"

#ifdef _WIN64
namespace debug_data
{
    CData<std::vector<std::string>> InputParamList;
    CData<PLAYER_TYPE> CurrentPlayer;
    CData<ITEM_TYPE> CurrentItemType;
    CData<std::tuple<PLAYER_TYPE, uint8_t, uint8_t>> PlayerHP;
    CData<std::string> SpecialCommand;
}
#endif
CData<uint8_t> Brightness;
CData<JsonDocument> JsonResponse;
CData<std::string> ClientName;
CData<STATE_TYPE> CurrentState;
CData<bool> GuiBlockState;
CData<int8_t> BatteryInd;

// Add auto reset state for custom data only
void UpdateAll()
{
    Brightness.ResetState();
    CurrentState.ResetState();
    GuiBlockState.ResetState();
    BatteryInd.ResetState();
#ifdef _WIN64
    debug_data::CurrentPlayer.ResetState();
    debug_data::CurrentItemType.ResetState();
    debug_data::PlayerHP.ResetState();
    debug_data::SpecialCommand.ResetState();
#endif
}

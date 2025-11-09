/**
 * @brief Common data
 */

#include "CommonData.h"

#ifdef _WIN64
CData<std::vector<std::string>> InputParamList;
CData<PLAYER_TYPE> CurrentPlayer;
CData<ITEM_TYPE> CurrentItemType;
CData<std::tuple<PLAYER_TYPE, uint8_t, uint8_t>> PlayerHP;
#endif
CData<uint8_t> Brightness;
CData<JsonDocument> JsonResponse;
CData<std::string> ClientName;
CData<STATE_TYPE> CurrentState;

// Add auto reset state for custom data only
void UpdateAll()
{
    Brightness.ResetState();
    CurrentState.ResetState();
#ifdef _WIN64
    CurrentPlayer.ResetState();
    CurrentItemType.ResetState();
    PlayerHP.ResetState();
#endif
}

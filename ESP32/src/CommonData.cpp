/**
 * @brief Common data
 */

#include "CommonData.h"

#ifdef _WIN64
CData<std::vector<std::string>> InputParamList;
CData<PLAYER_TYPE> CurrentPlayer;
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
#endif
}

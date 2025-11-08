/**
 * @brief Common data
 */

#include "CommonData.h"

#ifdef _WIN64
CData<std::vector<std::string>> InputParamList;
#endif
CData<uint8_t> Brightness;
CData<JsonDocument> JsonResponse;
CData<std::string> ClientName;
CData<PLAYER_TYPE> CurrentPlayer;
CData<STATE_TYPE> CurrentState;

// Add auto reset state for custom data only
void UpdateAll()
{
    Brightness.ResetState();
    CurrentPlayer.ResetState();
    CurrentState.ResetState();
}

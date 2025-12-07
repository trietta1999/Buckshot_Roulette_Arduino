/**
 * @brief Common data
 */

#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "ArduinoJson-v7.4.1.h"
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
extern CData<JsonDocument> JsonResponse;
extern CData<std::string> ClientName;
extern CData<STATE_TYPE> CurrentState;
extern CData<bool> GuiBlockState;
extern CData<int8_t> BatteryInd;

void UpdateAll();

#endif // !_COMMON_DATA_H

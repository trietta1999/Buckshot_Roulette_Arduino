/**
 * @brief Common data
 */

#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "ArduinoJson-v7.4.1.h"
#include "CommonDataType.h"
#include "CData.h"

#ifdef _WIN64
extern CData<std::vector<std::string>> InputParamList;
extern CData<PLAYER_TYPE> CurrentPlayer;
#endif
extern CData<uint8_t> Brightness;
extern CData<JsonDocument> JsonResponse;
extern CData<std::string> ClientName;
extern CData<STATE_TYPE> CurrentState;

void UpdateAll();

#endif // !_COMMON_DATA_H

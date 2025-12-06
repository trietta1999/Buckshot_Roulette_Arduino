/**
 * @brief Common library
 */

#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <lvgl.h>
#include "CommonDataType.h"

#define MAKE_DELAY_CB [](const void* data)

void Init();
void AutoUpdate();
void FSM();
void FSMTransit(STATE_TYPE next);

template<typename T>
T RandomRangeEnum(T a, T b)
{
    return (T)(((uint32_t)a + 1) + rand() % ((uint32_t)b - ((uint32_t)a + 1)));
}

int32_t RandomRangeNumber(int32_t a, int32_t b);
bool CheckObjectState(lv_obj_t* obj, lv_state_t state);
std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum);
void GetOrdinalNumber(uint8_t num, std::string& suffix);
void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms);
void BlockGui();
void UnblockGui();
std::string JoinString(std::string delimeter, const std::vector<std::string>& list);
#endif // !_COMMON_LIBRARY_H

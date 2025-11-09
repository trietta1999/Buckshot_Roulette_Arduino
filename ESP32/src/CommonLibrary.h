/**
 * @brief Common library
 */

#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <lvgl.h>
#include "CommonDataType.h"

void Init();
void AutoUpdate();

template<typename T>
T RandomRangeEnum(T a, T b)
{
    return (T)(((uint32_t)a + 1) + rand() % ((uint32_t)b - ((uint32_t)a + 1)));
}

int32_t RandomRangeNumber(int32_t a, int32_t b);
bool CheckObjectState(lv_obj_t* obj, lv_state_t state);
void PlayObjectRotatingAnimation(lv_obj_t* obj, int16_t endAngle, int16_t step);
std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum);
#endif // !_COMMON_LIBRARY_H

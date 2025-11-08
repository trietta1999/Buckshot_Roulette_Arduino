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
T RandomRange(T a, T b)
{
    return (T)(((uint32_t)a + 1) + rand() % ((uint32_t)b - ((uint32_t)a + 1)));
}

bool CheckObjectState(lv_obj_t* obj, lv_state_t state);
void PlayObjectRotate(lv_obj_t* obj, int16_t endAngle, int16_t step);
std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum);
#endif // !_COMMON_LIBRARY_H

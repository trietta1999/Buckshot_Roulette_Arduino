/**
 * @brief Common library header
 */

#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <lvgl.h>
#include "CommonDataType.h"

#define MAKE_DELAY_CB [](const void* data) // Macro for creating delay callback lambdas

void Init(); // Application-specific GUI initialization
void AutoUpdate(); // Automatic game state update logic
void FSM(); // Finite State Machine main processor
void FSMTransit(STATE_TYPE next); // Transition to a new FSM state

// Generates a random enum value within range [a+1, b-1]
template<typename T>
T RandomRangeEnum(T a, T b)
{
    return (T)(((uint32_t)a + 1) + rand() % ((uint32_t)b - ((uint32_t)a + 1)));
}

int32_t RandomRangeNumber(int32_t a, int32_t b); // Generates random number in range [a, b]
bool CheckObjectState(lv_obj_t* obj, lv_state_t state); // Checks if an LVGL object has a specific state
std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum); // Generates a randomized list of bullets
void GetOrdinalNumber(uint8_t num, std::string& suffix); // Gets English ordinal suffix (st, nd, rd, th)
void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms); // Executes a callback after delay
void BlockGui(); // Increments GUI block counter to disable interaction
void UnblockGui(); // Decrements GUI block counter
std::string JoinString(std::string delimeter, const std::vector<std::string>& list); // Joins vector of strings with delimiter

#endif // !_COMMON_LIBRARY_H

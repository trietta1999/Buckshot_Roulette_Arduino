/**
 * @brief Common library
 */

#include <algorithm>
#include <random>
#include "CommonLibrary.h"

int32_t RandomRangeNumber(int32_t a, int32_t b)
{
    return (a + rand() % (b + 1 - a));
}

bool CheckObjectState(lv_obj_t* obj, lv_state_t state)
{
    if ((lv_obj_get_state(obj) & state) == state)
    {
        return true;
    }

    return false;
}

void PlayObjectRotatingAnimation(lv_obj_t* obj, int16_t endAngle, int16_t step)
{
    struct angle_t
    {
        lv_obj_t* obj;
        int16_t startAngle;
        int16_t endAngle;
        int16_t step;
    };

    angle_t* data = (angle_t*)malloc(sizeof(angle_t));
    uint16_t startAngle = lv_obj_get_style_transform_rotation(obj, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Calculate angle
    if (startAngle < endAngle)
    {
        endAngle += step;
    }
    else if (startAngle > endAngle)
    {
        endAngle -= step;
    }

    // Set data
    data->obj = obj;
    data->startAngle = startAngle;
    data->endAngle = endAngle;
    data->step = step;

    // Create timer
    lv_timer_create([](lv_timer_t* timer) {
        auto data = (angle_t*)lv_timer_get_user_data(timer);

        lv_obj_set_style_transform_rotation(data->obj, data->startAngle, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (data->startAngle < data->endAngle)
        {
            data->startAngle += data->step;
        }
        else if (data->startAngle > data->endAngle)
        {
            data->startAngle -= data->step;
        }

        if (data->startAngle == data->endAngle)
        {
            lv_timer_del(timer);
            free(data);
            timer = nullptr;
        }
        }, 5, data);
}

std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum)
{
    std::vector<BULLET_TYPE> sample(maxNum);

    // Get number of Blank value
    uint8_t blankCount = RandomRangeNumber(1, maxNum - 1); // Must have at least 1 Live value

    // Assign Blank value to first element range
    for (uint8_t i = 0; i < blankCount; i++) {
        sample[i] = BULLET_TYPE::BLANK;
    }

    std::mt19937 gen(rand());
    std::shuffle(sample.begin(), sample.end(), gen); // Random shuffle

    // Fill Live value
    for (auto& item : sample)
    {
        if (item == BULLET_TYPE::MIN)
        {
            item = BULLET_TYPE::LIVE;
        }
    }

    return sample;
}

void GetOrdinalNumber(uint8_t num, std::string& suffix)
{
    if (num == 1)
    {
        suffix = "st";
    }
    else if (num == 2)
    {
        suffix = "nd";
    }
    else if (num == 3)
    {
        suffix = "rd";
    }
    else
    {
        suffix = "th";
    }
}

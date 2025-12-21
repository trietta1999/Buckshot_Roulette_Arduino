/**
 * @brief Common library
 */

#include <random>
#include "CommonLibrary.h"
#include "CommonData.h"

std::queue<bool> queueTimerExist = { };

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

std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum)
{
    std::vector<BULLET_TYPE> sample(maxNum), temp(maxNum);

    // Get number of Blank value
    uint8_t blankCount = RandomRangeNumber(1, maxNum - 1); // Must have at least 1 Live value

    // Assign Blank value to first element range
    for (uint8_t i = 0; i < blankCount; i++) {
        sample[i] = BULLET_TYPE::BLANK;
    }

    // Copy to temporary
    temp = sample;

    std::mt19937 gen(rand());

    // The random set must be different from the sample
    do
    {
        std::shuffle(temp.begin(), temp.end(), gen);
    } while (temp == sample);

    // Update sample
    sample = temp;

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

void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms)
{
    struct data_t
    {
        std::function<void(const void*)> func;
        void* data = nullptr;
    };

    data_t* ldata = new data_t;

    if (ldata)
    {
        ldata->func = func;
        ldata->data = data;

        BlockGui();

        lv_timer_create([](lv_timer_t* timer) {
            auto ldata = (data_t*)lv_timer_get_user_data(timer);

            ldata->func(ldata->data);

            delete ldata;

            UnblockGui();
            }, ms, ldata)->repeat_count = 1;
    }
}

void BlockGui()
{
    GuiBlockState.SetValue(true);
    queueTimerExist.push(true);
}

void UnblockGui()
{
    queueTimerExist.pop();

    if (!queueTimerExist.size())
    {
        GuiBlockState.SetValue(false);
    }
}

std::string JoinString(std::string delimeter, const std::vector<std::string>& list)
{
    std::string output;

    for (uint32_t i = 0; i < list.size(); i++)
    {
        if (i < list.size() - 1)
        {
            output += list[i] + delimeter;
        }
        else
        {
            output += list[i];
        }
    }

    return output;
}

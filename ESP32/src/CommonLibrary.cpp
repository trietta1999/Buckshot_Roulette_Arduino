/**
 * @brief Common library implementation
 */

#include <random>
#include "CommonLibrary.h"
#include "CommonData.h"

std::queue<bool> queueTimerExist = { }; // Queue to track nested GUI blocking timers

// Returns a random integer in range [a, b]
int32_t RandomRangeNumber(int32_t a, int32_t b)
{
    return (a + rand() % (b + 1 - a));
}

// Helper to check if an LVGL object has specific state flags
bool CheckObjectState(lv_obj_t* obj, lv_state_t state)
{
    if ((lv_obj_get_state(obj) & state) == state)
    {
        return true;
    }

    return false;
}

// Generates a randomized list of bullets (Live and Blank)
std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum)
{
    std::vector<BULLET_TYPE> sample(maxNum), temp(maxNum);

    // Randomize number of blanks (at least 1 live bullet guaranteed)
    uint8_t blankCount = RandomRangeNumber(1, maxNum - 1);

    // Fill the start of the list with blanks
    for (uint8_t i = 0; i < blankCount; i++) {
        sample[i] = BULLET_TYPE::BLANK;
    }

    temp = sample;

    std::mt19937 gen(rand());

    // Shuffle until the order is different from the initial sequential fill
    do
    {
        std::shuffle(temp.begin(), temp.end(), gen);
    } while (temp == sample);

    sample = temp;

    // Fill remaining unassigned slots (MIN/Default) with Live bullets
    for (auto& item : sample)
    {
        if (item == BULLET_TYPE::MIN)
        {
            item = BULLET_TYPE::LIVE;
        }
    }

    return sample;
}

// Converts a number to its ordinal suffix (e.g., 1 -> "st")
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

// Executes a callback function after a specified delay in ms, while blocking GUI
void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms)
{
    struct data_t // Context structure for the timer
    {
        std::function<void(const void*)> func;
        void* data = nullptr;
    };

    data_t* ldata = new data_t;

    if (ldata)
    {
        ldata->func = func;
        ldata->data = data;

        BlockGui(); // Disable user interaction during delay

        lv_timer_create([](lv_timer_t* timer) {
            auto ldata = (data_t*)lv_timer_get_user_data(timer);

            ldata->func(ldata->data); // Execute the stored function

            delete ldata; // Cleanup context

            UnblockGui(); // Re-enable user interaction
            }, ms, ldata)->repeat_count = 1; // Run only once
    }
}

// Sets the global GUI block state and tracks it in a queue
void BlockGui()
{
    GuiBlockState.SetValue(true);
    queueTimerExist.push(true);
}

// Removes a block from the queue and clears global state if empty
void UnblockGui()
{
    if (!queueTimerExist.empty())
    {
        queueTimerExist.pop();
    }

    if (queueTimerExist.empty())
    {
        GuiBlockState.SetValue(false);
    }
}

// Joins a vector of strings into a single string using a delimiter
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

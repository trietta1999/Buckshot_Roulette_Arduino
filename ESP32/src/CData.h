/**
 * @brief Common data class for handling state and value updates
 */

#ifndef _CDATA_H
#define _CDATA_H

template<class T>
class CData
{
private:
    T value; // Current value of the data
    T oldValue; // Previous value of the data
    bool state; // Flag indicating if the value has changed

public:
    CData()
    {
        this->state = false; // Initialize state as unchanged
    }

    ~CData() = default;

    T GetValue() // Returns the current value
    {
        return this->value;
    }

    T GetOldValue() // Returns the previous value
    {
        return this->oldValue;
    }

    void SetValue(T value) // Updates the value and sets changed flag if different
    {
        if (value != this->value)
        {
            this->oldValue = this->value;
            this->value = value;
            this->state = true;
        }
    }

    bool GetState() // Returns true if the value has changed since last reset
    {
        return this->state;
    }

    void ResetState() // Resets the change flag
    {
        if (this->state)
        {
            this->state = false;
        }
    }
};

#endif // !_CDATA_H

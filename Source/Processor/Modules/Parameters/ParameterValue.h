#pragma once

#include "Config.h"

struct ParameterValue {
    
public:
    float value = 0.0f;
    float midi = 0.0f;
    float percent= 0.0f;
    
    ParameterValue(float value = 0.0f) {
        set(value);
    }
    
    std::function<void(ParameterValue& param)> onChange = nullptr;
    
    ParameterValue& set(float value, bool viaPercentage = false) {
        if (viaPercentage) value = Config::MaxParameterValue * value;
        this->value = value;
        this->midi = clampValue(this->value, 0.0f, Config::MaxParameterValue);
        this->percent = clampValue(this->midi / Config::MaxParameterValue, 0.0f, 1.0f);
        
        if (onChange != nullptr) onChange(*this);
        return *this;
    }
    
    ParameterValue& increase(float value, bool viaPercentage = false) {
        if (viaPercentage) value = Config::MaxParameterValue * value;
        set(this->value + value);
        return *this;
    }
    
    ParameterValue& decrease(float value, bool viaPercentage = false) {
        if (viaPercentage) value = Config::MaxParameterValue * value;
        set(this->value - value);
        return *this;
    }
    
    ParameterValue& replace(const ParameterValue& replacer, bool viaPercentage = false) {
        float value = replacer.value;
        if (viaPercentage) value = Config::MaxParameterValue * value;
        set(value);
        return *this;
    }
    
    ParameterValue& operator =(const ParameterValue& replacer) {
        set(replacer.value);
        return *this;
    }
    
    ParameterValue& operator =(const ParameterValue* replacer) {
        set(replacer->value);
        return *this;
    }

private:

    float clampValue(float value, float min, float max) {
        if (value >= max) return max;
        if (value <= min) return min;
        return value;
    }

};

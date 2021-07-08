#pragma once

#include "Config.h"

class VolumeProcessor {
public:
    void process(float** buffRead, float** buffWrite, int sampleIndex, ParameterValue* volume) {
        buffWrite[Config::CHLeft][sampleIndex] = buffRead[Config::CHLeft][sampleIndex] * volume->percent;
        buffWrite[Config::CHRight][sampleIndex] = buffRead[Config::CHRight][sampleIndex] * volume->percent;
    }
    
};

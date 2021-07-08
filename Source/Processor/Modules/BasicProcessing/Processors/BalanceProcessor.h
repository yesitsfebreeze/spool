#pragma once

class BalanceProcessor {
public:
    void process(float** buffRead, float** buffWrite, int sampleIndex, ParameterValue* balance) {
        float balanceL = 1.0f;
        float balanceR = 1.0f;

        if (balance->percent > 0.5f) balanceL = (1 - balance->percent) * 2;
        if (balance->percent < 0.5f) balanceR = balance->percent * 2;
        
        float drySampleL = buffRead[Config::CHLeft][sampleIndex];
        float drySampleR = buffRead[Config::CHRight][sampleIndex];
        float tmpSampleL = drySampleL;
        float tmpSampleR = drySampleR;

        tmpSampleL *= balanceL;
        if (balanceR < 1) {
            tmpSampleL *= balanceR;
            tmpSampleL += drySampleR * (1 - balanceR);
        }
        
        tmpSampleR *= balanceR;
        if (balanceL < 1) {
            tmpSampleR *= balanceL;
            tmpSampleR += drySampleL * (1 - balanceL);
        }

        buffWrite[Config::CHLeft][sampleIndex] = tmpSampleL;
        buffWrite[Config::CHRight][sampleIndex] = tmpSampleR;
    }
    
};


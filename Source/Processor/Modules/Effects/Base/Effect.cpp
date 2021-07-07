#include "Effect.h"

#include "Processor/SpoolProcessor.h"

Effect::Effect(SpoolProcessor* processor, ParameterValue& wet, ParameterValue& paramA, ParameterValue& paramB, int index, int track, int sample) :
    processor(processor),
    wet(wet),
    paramA(paramA),
    paramB(paramB),
    index(index),
    track(track),
    sample(sample)
    
{
    setupDefaultParams();
    setupParamValueListeneres();
};

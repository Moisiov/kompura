#pragma once

#include <JuceHeader.h>

namespace Params {
    enum Names {
        Threshold,
        Attack,
        Release,
        Ratio,
        Bypass,
        InputGain,
        OutputGain
    };

    inline const std::map<Names, juce::String>& GetParams() {
        static const std::map<Names, juce::String> params = {
            { Threshold, "Threshold" },
            { Attack, "Attack" },
            { Release, "Release" },
            { Ratio, "Ratio" },
            { Bypass, "Bypass" },
            { InputGain, "In" },
            { OutputGain, "Out" }
        };
        return params;
    }
}
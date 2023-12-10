#pragma once

#include <JuceHeader.h>

struct CompressorBand {
public:
    juce::AudioParameterFloat* threshold = nullptr;
    juce::AudioParameterFloat* attack = nullptr;
    juce::AudioParameterFloat* release = nullptr;
    juce::AudioParameterFloat* ratio = nullptr;
    juce::AudioParameterBool* bypass = nullptr;

    void prepare(const juce::dsp::ProcessSpec& spec);

    void updateCompressorSettings();

    void process(juce::AudioBuffer<float>& buffer);

private:
    juce::dsp::Compressor<float> compressor;
};

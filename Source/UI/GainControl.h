#pragma once

#include <JuceHeader.h>
#include "../Params.h"

struct GainControl : juce::Component {
    GainControl(juce::AudioProcessorValueTreeState& apvts, const Params::Names name);

    void paint(juce::Graphics& g) override;
    void resized() override;
    juce::String getGainAmountText() const;

private:
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Label> gainAmount;
    std::unique_ptr<juce::Label> gainLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> gainAttachment;
};
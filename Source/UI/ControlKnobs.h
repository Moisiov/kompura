#pragma once

#include <JuceHeader.h>
#include "RotarySliderWithLabels.h"

struct ControlKnobs : juce::Component {
    ControlKnobs(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    std::unique_ptr<RotarySliderWithLabels> attackSlider, releaseSlider, thresholdSlider, ratioSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> attackAttachment, releaseAttachment, thresholdAttachment, ratioAttachment;
};
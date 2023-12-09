/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================

struct Placeholder : juce::Component {
    Placeholder();

    void paint(juce::Graphics& g) override {
        g.fillAll(customColor);
    }

    juce::Colour customColor;
};

class KompuraAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    KompuraAudioProcessorEditor (KompuraAudioProcessor&);
    ~KompuraAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KompuraAudioProcessor& audioProcessor;

    Placeholder logo, inputGainControl, outputGainControl, attackReleaseControls, thresholdRatioControls;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KompuraAudioProcessorEditor)
};

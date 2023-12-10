/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI/ControlKnobs.h"

struct Logo : juce::Component {
    void paint(juce::Graphics& g) override;
};

struct GainControls : juce::Slider {
    GainControls() : juce::Slider(juce::Slider::SliderStyle::LinearBarVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox) {}

	//void paint(juce::Graphics& g) override;
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

    GainControls inputGainControl, outputGainControl;
    ControlKnobs controlKnobs { audioProcessor.apvts };
    Logo logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KompuraAudioProcessorEditor)
};

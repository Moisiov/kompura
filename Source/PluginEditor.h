/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct LookAndFeel : juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        		float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted,
        				bool shouldDrawButtonAsDown) override;
};

struct RotarySliderWithLabels : juce::Slider {
    RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
            juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&rap),
        suffix(unitSuffix)
    {
		setLookAndFeel(&lnf);
	}

    ~RotarySliderWithLabels() {
		setLookAndFeel(nullptr);
	}

    struct LabelPos {
		float pos;
		juce::String label;
	};

	juce::Array<LabelPos> labels;

	void paint(juce::Graphics& g) override;

	juce::Rectangle<int> getSliderBounds() const;

	int getTextHeight() const { return 14; }
	juce::String getDisplayString() const;

private:
    LookAndFeel lnf;

	juce::RangedAudioParameter* param;
	juce::String suffix;
};

struct PowerButton : juce::ToggleButton {};

//==============================================================================

struct RotarySlider : juce::Slider {
    RotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        		juce::Slider::TextEntryBoxPosition::NoTextBox) {}
};

template<typename Attachment, typename APVTS, typename Params, typename ParamName, typename SliderType>
void makeAttachment(std::unique_ptr<Attachment>& attachment,
    APVTS& apvts,
    const Params& params,
    const ParamName& name,
    SliderType& slider)
{
    attachment = std::make_unique<Attachment>(apvts, params.at(name), slider);
}

struct Logo : juce::Component {
    void paint(juce::Graphics& g) override;
};

struct ControlKnobs : juce::Component {
    ControlKnobs(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    RotarySlider attackSlider, releaseSlider, thresholdSlider, ratioSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> attackAttachment, releaseAttachment, thresholdAttachment, ratioAttachment;
};

struct GainControls : juce::Component {
	void paint(juce::Graphics& g) override;
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

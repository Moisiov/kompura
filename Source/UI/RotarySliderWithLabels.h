#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"

struct RotarySliderWithLabels : juce::Slider {
	RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix, const juce::String& title) :
		juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
			juce::Slider::TextEntryBoxPosition::NoTextBox),
		param(&rap),
		suffix(unitSuffix)
	{
		setName(title);
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
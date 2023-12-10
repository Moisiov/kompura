#pragma once

#include <JuceHeader.h>

struct LookAndFeel : juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};
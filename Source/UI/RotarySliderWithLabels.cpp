#include "RotarySliderWithLabels.h"

void RotarySliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    getLookAndFeel().drawRotarySlider(
        g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this
    );

    auto bounds = getLocalBounds();
    g.setColour(Colours::white);
    g.drawFittedText(getName(), bounds.removeFromBottom(getTextHeight() * 2), Justification::centredTop, 1);
}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;
}

juce::String RotarySliderWithLabels::getDisplayString() const
{
    juce::String str;

    auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param);
    float val = getValue();

    float interval = floatParam->getNormalisableRange().interval;
    if (interval == std::floor(interval)) {
        str = juce::String((int)val);
    }
    else {
        str = juce::String(val, 1);
    }


    if (suffix.isNotEmpty())
    {
        str << " ";
        str << suffix;
    }

    return str;
}
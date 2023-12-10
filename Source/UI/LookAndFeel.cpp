#include "LookAndFeel.h"
#include "RotarySliderWithLabels.h"

void LookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, width, height);

    auto enabled = slider.isEnabled();

    slider.setColour(Slider::thumbColourId, Colours::white);
    LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);

    auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider);
    Rectangle<float> r;
    auto center = bounds.getCentre();
    r.setLeft(center.getX() - 2);
    r.setRight(center.getX() + 2);
    r.setTop(bounds.getY());
    r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);
    g.setFont(rswl->getTextHeight());
    auto text = rswl->getDisplayString();
    auto strWidth = g.getCurrentFont().getStringWidth(text);

    r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
    r.setCentre(bounds.getCentre());

    g.setColour(enabled ? Colours::black : Colours::darkgrey);
    g.fillRect(r);

    g.setColour(enabled ? Colours::white : Colours::lightgrey);
    g.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
}

void LookAndFeel::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& toggleButton,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    using namespace juce;
    Path powerButton;

    auto bounds = toggleButton.getLocalBounds();

    auto size = jmin(bounds.getWidth(), bounds.getHeight()) - 6;
    auto r = bounds.withSizeKeepingCentre(size, size).toFloat();

    float ang = 30.f; //30.f;

    size -= 6;

    powerButton.addCentredArc(r.getCentreX(),
        r.getCentreY(),
        size * 0.5,
        size * 0.5,
        0.f,
        degreesToRadians(ang),
        degreesToRadians(360.f - ang),
        true);

    powerButton.startNewSubPath(r.getCentreX(), r.getY());
    powerButton.lineTo(r.getCentre());

    PathStrokeType pst(2.f, PathStrokeType::JointStyle::curved);

    auto color = toggleButton.getToggleState() ? Colours::dimgrey : Colour(0u, 172u, 1u);

    g.setColour(color);
    g.strokePath(powerButton, pst);
    g.drawEllipse(r, 2);
}
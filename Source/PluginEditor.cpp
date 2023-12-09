/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
//==============================================================================
void RotarySliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this);

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

void Logo::paint(juce::Graphics& g)
{
    juce::Image logo = juce::ImageCache::getFromMemory(BinaryData::logo_jpg, BinaryData::logo_jpgSize);
    g.drawImage(logo, getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
}

ControlKnobs::ControlKnobs(juce::AudioProcessorValueTreeState& apvts)
{
    using namespace Params;
    const auto& params = GetParams();

    auto getParamHelper = [&params, &apvts](const auto& name) -> auto&
    {
        return getParam(apvts, params, name);
    };

    attackSlider = std::make_unique<RotarySliderWithLabels>(getParamHelper(Names::Attack), "ms", params.at(Names::Attack));
    releaseSlider = std::make_unique<RotarySliderWithLabels>(getParamHelper(Names::Release), "ms", params.at(Names::Release));
    thresholdSlider = std::make_unique<RotarySliderWithLabels>(getParamHelper(Names::Threshold), "dB", params.at(Names::Threshold));
    ratioSlider = std::make_unique<RotarySliderWithLabels>(getParamHelper(Names::Ratio), "", params.at(Names::Ratio));

    auto makeAttachmentHelper = [&params, &apvts](auto& attachment, const auto& name, auto& slider)
    {
            makeAttachment(attachment, apvts, params, name, slider);
	};

    makeAttachmentHelper(attackAttachment, Names::Attack, *attackSlider);
    makeAttachmentHelper(releaseAttachment, Names::Release, *releaseSlider);
    makeAttachmentHelper(thresholdAttachment, Names::Threshold, *thresholdSlider);
    makeAttachmentHelper(ratioAttachment, Names::Ratio, *ratioSlider);

    addAndMakeVisible(*attackSlider);
    addAndMakeVisible(*releaseSlider);
    addAndMakeVisible(*thresholdSlider);
    addAndMakeVisible(*ratioSlider);
}

void ControlKnobs::paint(juce::Graphics& g)
{ 

}

void ControlKnobs::resized()
{
    using namespace juce;
    auto bounds = getLocalBounds();
    FlexBox attackRelease;
    attackRelease.flexDirection = FlexBox::Direction::row;
    attackRelease.flexWrap = FlexBox::Wrap::noWrap;
    attackRelease.items.add(FlexItem(*attackSlider).withFlex(1.0f));
    attackRelease.items.add(FlexItem(*releaseSlider).withFlex(1.0f));

    FlexBox thresholdRatio;
    thresholdRatio.items.add(FlexItem(*thresholdSlider).withFlex(1.0f));
    thresholdRatio.items.add(FlexItem(*ratioSlider).withFlex(1.0f));

    attackRelease.performLayout(bounds.removeFromTop(bounds.getHeight()/2).toFloat());
    thresholdRatio.performLayout(bounds.toFloat());
}

void GainControls::paint(juce::Graphics& g)
{
}

//==============================================================================
KompuraAudioProcessorEditor::KompuraAudioProcessorEditor (KompuraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(logo);
    addAndMakeVisible(inputGainControl);
    addAndMakeVisible(outputGainControl);
    addAndMakeVisible(controlKnobs);
    setSize (400, 300);
}

KompuraAudioProcessorEditor::~KompuraAudioProcessorEditor()
{
}

//==============================================================================
void KompuraAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void KompuraAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    logo.setBounds(bounds.removeFromTop(32));
    inputGainControl.setBounds(bounds.removeFromLeft(32));
    outputGainControl.setBounds(bounds.removeFromRight(32));
    controlKnobs.setBounds(bounds);
}
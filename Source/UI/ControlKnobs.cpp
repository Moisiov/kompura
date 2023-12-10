#include "ControlKnobs.h"
#include "../Params.h"
#include "Utility.h"

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

    attackRelease.performLayout(bounds.removeFromTop(bounds.getHeight() / 2).toFloat());
    thresholdRatio.performLayout(bounds.toFloat());
}
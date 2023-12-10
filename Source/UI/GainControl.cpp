#include "GainControl.h"
#include "Utility.h"

GainControl::GainControl(juce::AudioProcessorValueTreeState& apvts, const Params::Names name)
{
	using namespace juce;
	using namespace Params;
	const auto& params = GetParams();

	gainSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
	gainAmount = std::make_unique<Label>("GainAmount", getGainAmountText());
	gainAmount->setJustificationType(Justification::centred);
	gainLabel = std::make_unique<Label>("GainLabel", params.at(name));
	gainLabel->setJustificationType(Justification::centred);

	makeAttachment(gainAttachment, apvts, params, name, *gainSlider);

	addAndMakeVisible(*gainSlider);
	addAndMakeVisible(*gainAmount);
	addAndMakeVisible(*gainLabel);
}

void GainControl::paint(juce::Graphics& g)
{
	using namespace juce;
	gainSlider->setColour(Slider::ColourIds::thumbColourId, Colours::white);
	gainSlider->paint(g);
	gainAmount->setText(getGainAmountText(), NotificationType::dontSendNotification);
}

void GainControl::resized()
{
	using namespace juce;
	auto bounds = getLocalBounds();
	FlexBox flexBox;
	flexBox.flexDirection = FlexBox::Direction::column;
	flexBox.flexWrap = FlexBox::Wrap::noWrap;
	flexBox.alignContent = FlexBox::AlignContent::center;
	flexBox.items.add(FlexItem(*gainSlider).withFlex(1.0f));
	flexBox.items.add(FlexItem(*gainAmount).withHeight(24));
	flexBox.items.add(FlexItem(*gainLabel).withHeight(24));
	flexBox.performLayout(bounds.toFloat());
}

juce::String GainControl::getGainAmountText() const
{
	using namespace juce;
	String str = String(gainSlider->getValue(), 1);
	str << " dB";
	return str;
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

Placeholder::Placeholder() {
    juce::Random r;
    customColor = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
}

//==============================================================================
KompuraAudioProcessorEditor::KompuraAudioProcessorEditor (KompuraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(logo);
    addAndMakeVisible(inputGainControl);
    addAndMakeVisible(outputGainControl);
    addAndMakeVisible(attackReleaseControls);
    addAndMakeVisible(thresholdRatioControls);
    setSize (400, 300);
}

KompuraAudioProcessorEditor::~KompuraAudioProcessorEditor()
{
}

//==============================================================================
void KompuraAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void KompuraAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    inputGainControl.setBounds(bounds.removeFromLeft(32));
    outputGainControl.setBounds(bounds.removeFromRight(32));
    logo.setBounds(bounds.removeFromTop(32));
    thresholdRatioControls.setBounds(bounds.removeFromTop(bounds.getHeight()/2));
    attackReleaseControls.setBounds(bounds);
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void Logo::paint(juce::Graphics& g)
{
    juce::Image logo = juce::ImageCache::getFromMemory(BinaryData::logo_jpg, BinaryData::logo_jpgSize);
    g.drawImage(logo, getLocalBounds().toFloat(), juce::RectanglePlacement::centred);
}

//void GainControls::paint(juce::Graphics& g)
//{
//}

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
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

KompuraAudioProcessorEditor::KompuraAudioProcessorEditor (KompuraAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(logo);
    addAndMakeVisible(inputGainControl);
    addAndMakeVisible(outputGainControl);
    addAndMakeVisible(controlKnobs);
    setSize (500, 300);
}

KompuraAudioProcessorEditor::~KompuraAudioProcessorEditor()
{
}

void KompuraAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void KompuraAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    inputGainControl.setBounds(bounds.removeFromLeft(64));
    outputGainControl.setBounds(bounds.removeFromRight(64));
    logo.setBounds(bounds.removeFromTop(32));
    controlKnobs.setBounds(bounds);
}
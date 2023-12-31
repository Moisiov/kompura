/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
KompuraAudioProcessor::KompuraAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    using namespace Params;
    const auto& params = GetParams();

    auto floatHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName) {
		param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(paramName));
	};

    auto boolHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName) {
        param = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(paramName));
    };

    floatHelper(inputGainParam, params.at(Names::InputGain));
    floatHelper(compressor.threshold, params.at(Names::Threshold));
    floatHelper(compressor.attack, params.at(Names::Attack));
    floatHelper(compressor.release, params.at(Names::Release));
    floatHelper(compressor.ratio, params.at(Names::Ratio));
    floatHelper(outputGainParam, params.at(Names::OutputGain));
    boolHelper(compressor.bypass, params.at(Names::Bypass));
}

KompuraAudioProcessor::~KompuraAudioProcessor()
{
}

//==============================================================================
const juce::String KompuraAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KompuraAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KompuraAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KompuraAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KompuraAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KompuraAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KompuraAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KompuraAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KompuraAudioProcessor::getProgramName (int index)
{
    return {};
}

void KompuraAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KompuraAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();

    inputGain.prepare(spec);
    outputGain.prepare(spec);

    inputGain.setRampDurationSeconds(0.05);
    outputGain.setRampDurationSeconds(0.05);

    compressor.prepare(spec);
}

void KompuraAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KompuraAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KompuraAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateState();

    applyGain(buffer, inputGain);

    compressor.process(buffer);

    applyGain(buffer, outputGain);
}

void KompuraAudioProcessor::updateState()
{
    compressor.updateCompressorSettings();
    inputGain.setGainDecibels(inputGainParam->get());
    outputGain.setGainDecibels(outputGainParam->get());
}

//==============================================================================
bool KompuraAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KompuraAudioProcessor::createEditor()
{
    return new KompuraAudioProcessorEditor(*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void KompuraAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void KompuraAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid())
    {
		apvts.replaceState(tree);
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout KompuraAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;

    using namespace juce;
    using namespace Params;
    const auto& params = GetParams();

    auto gainRange = NormalisableRange<float>(-24.f, 24.f, 0.1, 1);
    auto attackRange = NormalisableRange<float>(0, 500, 1, 1);
    auto releaseRange = NormalisableRange<float>(5, 500, 1, 1);

    layout.add(std::make_unique<AudioParameterBool>(
        params.at(Names::Bypass),
        params.at(Names::Bypass),
        false
    ));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::InputGain),
        params.at(Names::InputGain),
		gainRange,
		0
	));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::Threshold),
        params.at(Names::Threshold),
        NormalisableRange<float>(-60, 12, 0.1, 1),
        0
    ));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::Attack),
        params.at(Names::Attack),
        attackRange,
        50
    ));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::Release),
        params.at(Names::Release),
		releaseRange,
		250
    ));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::Ratio),
        params.at(Names::Ratio),
		NormalisableRange<float>(1, 20, 0.1, 1),
		2
	));

    layout.add(std::make_unique<AudioParameterFloat>(
        params.at(Names::OutputGain),
        params.at(Names::OutputGain),
        gainRange,
        0)
    );

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KompuraAudioProcessor();
}

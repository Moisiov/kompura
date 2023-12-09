/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

struct CompressorBand {
public:
    juce::AudioParameterFloat* threshold = nullptr;
    juce::AudioParameterFloat* attack = nullptr;
    juce::AudioParameterFloat* release = nullptr;
    juce::AudioParameterFloat* ratio = nullptr;
    juce::AudioParameterBool* bypass = nullptr;

    void prepare(const juce::dsp::ProcessSpec& spec) {
		compressor.prepare(spec);
	}

    void updateCompressorSettings() {
		compressor.setThreshold(threshold->get());
		compressor.setAttack(attack->get());
		compressor.setRelease(release->get());
		compressor.setRatio(ratio->get());
	}

    void process(juce::AudioBuffer<float>& buffer) {
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        context.isBypassed = bypass->get();

        compressor.process(context);
    }

private :
	juce::dsp::Compressor<float> compressor;
};

class KompuraAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    KompuraAudioProcessor();
    ~KompuraAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    using APVTS = juce::AudioProcessorValueTreeState;
    static APVTS::ParameterLayout createParameterLayout();
    APVTS apvts { *this, nullptr, "Parameters", createParameterLayout() };

private:
    CompressorBand compressor;

    juce::dsp::Gain<float> inputGain, outputGain;
    juce::AudioParameterFloat* inputGainParam = nullptr;
    juce::AudioParameterFloat* outputGainParam = nullptr;

    template<typename T, typename U>
    void applyGain(T& buffer, U& dsp) {
		auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        dsp.process(context);
	}

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KompuraAudioProcessor)
};

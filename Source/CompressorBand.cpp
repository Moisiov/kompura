#include "CompressorBand.h"

void CompressorBand::prepare(const juce::dsp::ProcessSpec& spec) {
    compressor.prepare(spec);
}

void CompressorBand::updateCompressorSettings() {
    compressor.setThreshold(threshold->get());
    compressor.setAttack(attack->get());
    compressor.setRelease(release->get());
    compressor.setRatio(ratio->get());
}

void CompressorBand::process(juce::AudioBuffer<float>& buffer) {
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    context.isBypassed = bypass->get();

    compressor.process(context);
}
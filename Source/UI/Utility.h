#pragma once

#include <JuceHeader.h>

template<typename Attachment, typename APVTS, typename Params, typename ParamName, typename SliderType>
void makeAttachment(std::unique_ptr<Attachment>& attachment,
    APVTS& apvts,
    const Params& params,
    const ParamName& name,
    SliderType& slider)
{
    attachment = std::make_unique<Attachment>(apvts, params.at(name), slider);
}

template<typename APVTS, typename Params, typename Name>
juce::RangedAudioParameter& getParam(APVTS& apvts, const Params& params, const Name& name)
{
    auto param = apvts.getParameter(params.at(name));
    jassert(param != nullptr);
    return *param;
}
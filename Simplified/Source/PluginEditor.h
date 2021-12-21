/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/XyPad.h"

//==============================================================================
/**
*/
class XyPadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    XyPadAudioProcessorEditor (XyPadAudioProcessor&);
    ~XyPadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	
    XyPadAudioProcessor& audioProcessor;

    Slider gainSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    Slider panSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    SliderAttachment gainAttachment, panAttachment;
    Label gainLabel{ "gainLabel", "Gain" };
    Label panLabel{ "panLabel", "Pan" };
    Gui::XyPad xyPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XyPadAudioProcessorEditor)
};

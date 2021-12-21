#include "PluginProcessor.h"
#include "PluginEditor.h"

XyPadAudioProcessorEditor::XyPadAudioProcessorEditor (XyPadAudioProcessor& p) :
    AudioProcessorEditor (&p),
    audioProcessor (p),
    parameters(p.getApvts()),
    panel1(parameters),
    panel2(parameters)
{
    addAndMakeVisible(tabbedComponent);
    tabbedComponent.addTab("Single Slider", Colours::transparentBlack, &panel1, false);
    tabbedComponent.addTab("Multiple Configurable Sliders", Colours::transparentBlack, &panel2, false);

    setSize (800, 400);
    setResizable(true, true);
}

//==============================================================================
void XyPadAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setGradientFill(ColourGradient{ Colours::darkgrey.brighter(0.2f), getLocalBounds().toFloat().getCentre(), Colours::darkgrey.darker(0.8f), {}, true });
    g.fillRect(getLocalBounds());    
}

void XyPadAudioProcessorEditor::resized()
{
    tabbedComponent.setBounds(getLocalBounds());
}

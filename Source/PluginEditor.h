#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/XyPad.h"
#include "Panel/Panel1.h"
#include "Panel/Panel2.h"

class XyPadAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    XyPadAudioProcessorEditor (XyPadAudioProcessor&);    

    void paint (Graphics&) override;
    void resized() override;
private:   
    XyPadAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& parameters;

    Gui::Panel1 panel1;
    Gui::Panel2 panel2;
    TabbedComponent tabbedComponent{ TabbedButtonBar::TabsAtTop };     
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XyPadAudioProcessorEditor)
};

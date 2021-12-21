/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
XyPadAudioProcessor::XyPadAudioProcessor() :
	AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true)),
	parameters(*this, nullptr, "xypad", {
		std::make_unique<AudioParameterFloat>("pan", "Pan", NormalisableRange<float>{-1.f, 1.f, 0.01f}, 0.f),
		std::make_unique<AudioParameterFloat>("gain", "Gain", NormalisableRange<float>{-60.f, 0.f, 0.01f}, 15.f)
	})
{
	parameters.addParameterListener("gain", this);
	parameters.addParameterListener("pan", this);
}

XyPadAudioProcessor::~XyPadAudioProcessor()
{
	parameters.removeParameterListener("gain", this);
	parameters.removeParameterListener("pan", this);
}

//==============================================================================
const String XyPadAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool XyPadAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
	return false;
#endif
}

bool XyPadAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
	return false;
#endif
}

bool XyPadAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
	return false;
#endif
}

double XyPadAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int XyPadAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int XyPadAudioProcessor::getCurrentProgram()
{
	return 0;
}

void XyPadAudioProcessor::setCurrentProgram(int index)
{
}

const String XyPadAudioProcessor::getProgramName(int index)
{
	return {};
}

void XyPadAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void XyPadAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, getTotalNumInputChannels() };
	gainProcessor.prepare(spec);
	panProcessor.prepare(spec);

	gainProcessor.setGainDecibels(parameters.getRawParameterValue("gain")->load());
	panProcessor.setPan(parameters.getRawParameterValue("pan")->load());	
}

void XyPadAudioProcessor::releaseResources()
{
	gainProcessor.reset();
	panProcessor.reset();
}

bool XyPadAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
	
	return true;
}

void XyPadAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;

	dsp::AudioBlock<float> block{ buffer };
	dsp::ProcessContextReplacing<float> context{ block };
	gainProcessor.process(context);
	panProcessor.process(context);
}

bool XyPadAudioProcessor::hasEditor() const
{
	return true; 
}

AudioProcessorEditor* XyPadAudioProcessor::createEditor()
{
	return new XyPadAudioProcessorEditor(*this);
}

void XyPadAudioProcessor::getStateInformation(MemoryBlock& destData)
{
}

void XyPadAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

void XyPadAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
	if (parameterID.equalsIgnoreCase("gain"))
		gainProcessor.setGainDecibels(newValue);
	if (parameterID.equalsIgnoreCase("pan"))
		panProcessor.setPan(newValue);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new XyPadAudioProcessor();
}

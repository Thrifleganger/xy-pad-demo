#pragma once

#include <JuceHeader.h>

#include "../Component/XyPad.h"

namespace Gui
{
	class Panel1 : public Component
	{
	public:
		Panel1(AudioProcessorValueTreeState& apvts) :
			parameters(apvts),
			gainAttachment(parameters, "gain", gainSlider),
			panAttachment(parameters, "pan", panSlider)			
		{
			addAndMakeVisible(gainSlider);
			addAndMakeVisible(panSlider);
			addAndMakeVisible(gainLabel);
			addAndMakeVisible(panLabel);
			addAndMakeVisible(xyPad);

			const auto configureSliderColour = [](Slider& slider, const Colour& colour)
			{
				slider.setColour(Slider::ColourIds::rotarySliderFillColourId, colour.withBrightness(0.6f));
				slider.setColour(Slider::ColourIds::thumbColourId, colour);
			};
			configureSliderColour(gainSlider, Colours::violet);
			configureSliderColour(panSlider, Colours::cyan);
			gainSlider.setTextValueSuffix(" dB");			

			gainLabel.setJustificationType(Justification::centred);
			panLabel.setJustificationType(Justification::centred);
			gainLabel.attachToComponent(&gainSlider, false);
			panLabel.attachToComponent(&panSlider, false);

			xyPad.registerSlider(&gainSlider, Gui::XyPad::Axis::Y);
			xyPad.registerSlider(&panSlider, Gui::XyPad::Axis::X);
		}

		~Panel1()
		{
			xyPad.deregisterSlider(&gainSlider);
			xyPad.deregisterSlider(&panSlider);
		}

		void paint(Graphics& g) override
		{
			const DropShadow dropShadow{ Colours::black.withAlpha(0.5f), 10, {} };
			dropShadow.drawForRectangle(g, xyPad.getBoundsInParent());
		}

		void resized() override
		{
			const auto container = getLocalBounds().reduced(20);
			auto bounds = container;

			const auto createCenteredBounds = [](Component& component, const Rectangle<int>& bounds)
			{
				const auto size = jmin(bounds.getWidth(), bounds.getHeight());
				component.setBounds(bounds.withSizeKeepingCentre(size, size));
			};			
			createCenteredBounds(gainSlider, bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(20));
			createCenteredBounds(xyPad, bounds.removeFromLeft(container.proportionOfWidth(0.5f)).reduced(20));
			createCenteredBounds(panSlider, bounds.reduced(20));
		}

	private:
		using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
		AudioProcessorValueTreeState& parameters;
		
		Label gainLabel{ "gainLabel", "Gain" }, panLabel{ "panLabel", "Pan" };
		Slider gainSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
		Slider panSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
		SliderAttachment gainAttachment, panAttachment;
		Gui::XyPad xyPad;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Panel1)
	};
}

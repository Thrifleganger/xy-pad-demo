#pragma once

#include <JuceHeader.h>


#include "../Component/ConfigurableSlider.h"
#include "../Component/XyPad.h"

namespace Gui
{
	class Panel2 : public Component
	{
	public:
		Panel2(AudioProcessorValueTreeState& apvts) :
			parameters(apvts),
			gainAttachment(parameters, "gain", gainSlider),
			panAttachment(parameters, "pan", panSlider)
		{
			addAndMakeVisible(gainSlider);
			addAndMakeVisible(panSlider);
			addAndMakeVisible(gainLabel);
			addAndMakeVisible(panLabel);
			addAndMakeVisible(xyPad);

			addAndMakeVisible(slider1);
			addAndMakeVisible(slider2);
			addAndMakeVisible(slider3);
			addAndMakeVisible(slider4);

			gainSlider.setTextValueSuffix(" dB");
			configureSliderColour(gainSlider, Colours::violet);
			configureSliderColour(slider1, Colours::violet);
			configureSliderColour(slider2, Colours::violet);
			configureSliderColour(panSlider, Colours::cyan);
			configureSliderColour(slider3, Colours::cyan);
			configureSliderColour(slider4, Colours::cyan);			

			gainLabel.setJustificationType(Justification::centred);
			panLabel.setJustificationType(Justification::centred);
			gainLabel.attachToComponent(&gainSlider, false);
			panLabel.attachToComponent(&panSlider, false);

			xyPad.registerSlider(&gainSlider, Gui::XyPad::Axis::Y);
			xyPad.registerSlider(&slider1, Gui::XyPad::Axis::Y);
			xyPad.registerSlider(&slider2, Gui::XyPad::Axis::Y);
			
			xyPad.registerSlider(&panSlider, Gui::XyPad::Axis::X);
			xyPad.registerSlider(&slider3, Gui::XyPad::Axis::X);
			xyPad.registerSlider(&slider4, Gui::XyPad::Axis::X);
		}

		~Panel2()
		{
			xyPad.deregisterSlider(&gainSlider);
			xyPad.deregisterSlider(&panSlider);
			xyPad.deregisterSlider(&slider1);
			xyPad.deregisterSlider(&slider2);
			xyPad.deregisterSlider(&slider3);
			xyPad.deregisterSlider(&slider4);
		}

		void paint(Graphics& g) override
		{
			const DropShadow dropShadow{ Colours::black.withAlpha(0.5f), 10, {} };
			dropShadow.drawForRectangle(g, xyPad.getBoundsInParent());
		}

		void configureSliderColour(Slider& slider, const Colour& colour)
		{
			slider.setColour(Slider::ColourIds::rotarySliderFillColourId, colour.withBrightness(0.6f));
			slider.setColour(Slider::ColourIds::thumbColourId, colour);
		};

		void resized() override
		{
			const auto container = getLocalBounds().reduced(20);
			auto bounds = container;

			const auto createCenteredBounds = [](Component& component, const Rectangle<int>& bounds)
			{
				const auto size = jmin(bounds.getWidth(), bounds.getHeight());
				component.setBounds(bounds.withSizeKeepingCentre(size, size));
			};
			auto leftSliders = bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(20);
			createCenteredBounds(gainSlider, leftSliders.removeFromTop(leftSliders.proportionOfHeight(0.7f)));
			createCenteredBounds(slider1, leftSliders.removeFromLeft(leftSliders.proportionOfWidth(0.5f)));
			createCenteredBounds(slider2, leftSliders);

			createCenteredBounds(xyPad, bounds.removeFromLeft(container.proportionOfWidth(0.5f)).reduced(20));

			auto rightSliders = bounds.reduced(20);
			createCenteredBounds(panSlider, rightSliders.removeFromTop(rightSliders.proportionOfHeight(0.7f)));
			createCenteredBounds(slider3, rightSliders.removeFromLeft(rightSliders.proportionOfWidth(0.5f)));
			createCenteredBounds(slider4, rightSliders);			
		}

	private:
		using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
		AudioProcessorValueTreeState& parameters;

		Label gainLabel{ "gainLabel", "Gain" }, panLabel{ "panLabel", "Pan" };
		Slider gainSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
		Slider panSlider{ Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
		SliderAttachment gainAttachment, panAttachment;
		Gui::XyPad xyPad;

		ConfigurableSlider slider1{ *this, xyPad };
		ConfigurableSlider slider2{ *this, xyPad };
		ConfigurableSlider slider3{ *this, xyPad };
		ConfigurableSlider slider4{ *this, xyPad };

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Panel2)
	};
}

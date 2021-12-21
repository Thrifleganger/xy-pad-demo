#pragma once

#include <JuceHeader.h>

#include "XyPad.h"

namespace Gui
{
	class ConfigurableSlider : public Slider
	{
	public:		
		ConfigurableSlider(Component& parent, XyPad& xyPad) :
			Slider(RotaryHorizontalVerticalDrag, NoTextBox),
			parent(parent),
			xyPad(xyPad)
		{					
			PopupMenu::Item registerXAxis{ "Register to X Axis" };
			registerXAxis.action = [&]
			{
				xyPad.deregisterSlider(this);
				xyPad.registerSlider(this, XyPad::Axis::X);
				configureSliderColour(Colours::cyan);
			};
			PopupMenu::Item registerYAxis{ "Register to Y Axis" };
			registerYAxis.action = [&]
			{
				xyPad.deregisterSlider(this);
				xyPad.registerSlider(this, XyPad::Axis::Y);
				configureSliderColour(Colours::violet);
			};
			PopupMenu::Item reset("Reset");
			reset.action = [&]
			{
				xyPad.deregisterSlider(this);
				configureSliderColour(Colours::grey);
			};

			popupMenu.addItem(registerXAxis);
			popupMenu.addItem(registerYAxis);
			popupMenu.addSeparator();
			popupMenu.addItem(reset);
		}

	private:
		void mouseDown(const MouseEvent& event) override
		{
			const auto modifiers = ComponentPeer::getCurrentModifiersRealtime();
			if (modifiers.isRightButtonDown())
			{
				const auto options = PopupMenu::Options().withParentComponent(&parent);
				popupMenu.showMenuAsync(options);
			}
			else
			{
				Slider::mouseDown(event);
			}
		}

		void configureSliderColour(const Colour& colour)
		{
			setColour(rotarySliderFillColourId, colour.withBrightness(0.6f));
			setColour(thumbColourId, colour);
		}
		
		Component& parent;
		XyPad& xyPad;
		PopupMenu popupMenu;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigurableSlider)
	};
}

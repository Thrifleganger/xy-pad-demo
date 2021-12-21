#include "XyPad.h"

namespace Gui
{
	XyPad::Thumb::Thumb()
	{
		constrainer.setMinimumOnscreenAmounts(thumbWidth, thumbWidth, thumbWidth, thumbWidth);
	}

	void XyPad::Thumb::mouseDown(const MouseEvent& event)
	{
		dragger.startDraggingComponent(this, event);
	}

	void XyPad::Thumb::mouseDrag(const MouseEvent& event)
	{
		dragger.dragComponent(this, event, &constrainer);
		if (moveCallback)
			moveCallback(getPosition().toDouble());
	}

	void XyPad::Thumb::paint(Graphics& g)
	{		
		g.setColour(Colours::white);
		g.fillEllipse(getLocalBounds().toFloat());
	}

	XyPad::XyPad()
	{
		addAndMakeVisible(thumb);
		thumb.moveCallback = [&](Point<double> position)
		{
			const std::lock_guard<std::mutex> lock(mutex);
			const auto bounds = getLocalBounds().toDouble();
			const auto w = static_cast<double>(thumbWidth);
			for (auto* slider : xSliders)
			{
				slider->setValue(jmap(position.getX(), 0.0, bounds.getWidth() - w, slider->getMinimum(), slider->getMaximum()));
			}
			for (auto* slider : ySliders)
			{
				slider->setValue(jmap(position.getY(), bounds.getHeight() - w, 0.0, slider->getMinimum(), slider->getMaximum()));
			}
			repaint();
		};
	}

	void XyPad::registerSlider(Slider* slider, Axis axis)
	{
		slider->addListener(this);
		const std::lock_guard<std::mutex> lock(mutex);
		if (axis == X)
			xSliders.push_back(slider);
		if (axis == Y)
			ySliders.push_back(slider);		
	}

	void XyPad::deregisterSlider(Slider* slider)
	{
		slider->removeListener(this);
		const std::lock_guard<std::mutex> lock(mutex);
		xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
		ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
	}

	void XyPad::resized()
	{		
		thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbWidth, thumbWidth));
		if (!xSliders.empty())
			sliderValueChanged(xSliders[0]);
		if (!ySliders.empty())
			sliderValueChanged(ySliders[0]);
	}

	void XyPad::paint(Graphics& g)
	{
		const auto bounds = getLocalBounds().toFloat();		
		
		g.setGradientFill(ColourGradient{ Colours::black.brighter(0.2f), bounds.getTopLeft(), Colours::black.brighter(0.1f), bounds.getBottomLeft(), false });
		g.fillRoundedRectangle(bounds, 10);
		
		const auto r = thumbWidth / 2.f;
		const auto thumbX = thumb.getX() + r;
		const auto thumbY = thumb.getY() + r;

		g.setColour(Colours::violet);
		g.drawLine(Line<float> { {0.f, thumbY}, { bounds.getWidth(), thumbY } });

		g.setColour(Colours::cyan);
		g.drawLine(Line<float> { {thumbX, 0.f}, { thumbX, bounds.getHeight() } });

		Path circle;
		circle.addEllipse(thumb.getBoundsInParent().toFloat());
		const DropShadow dropShadow;
		dropShadow.drawForPath(g, circle);
	}

	void XyPad::sliderValueChanged(Slider* slider)
	{
		if (thumb.isMouseOverOrDragging(false))
			return;
		const auto bounds = getLocalBounds().toDouble();
		const auto w = static_cast<double>(thumbWidth);
		const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
		if (isXAxisSlider)
		{
			thumb.setTopLeftPosition(
				jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), 0.0, bounds.getWidth() - w),
				thumb.getY());
		} else
		{
			thumb.setTopLeftPosition(
				thumb.getX(),
				jmap(slider->getValue(), slider->getMinimum(), slider->getMaximum(), bounds.getHeight() - w, 0.0));
		}
		repaint();
	}
}

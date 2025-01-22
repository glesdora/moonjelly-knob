#ifndef MOONJLED_H
#define MOONJLED_H

#pragma once

#include <JuceHeader.h>
#include "MoonJPalette.h"

using namespace juce;

class MoonJLed : public Component
{
public:
	MoonJLed(bool isLine) : alpha(0.0f), isLine(isLine)
	{
	}

	void paint(Graphics& g) override
	{
		auto fbounds = getLocalBounds().toFloat();
		float glowradius = 0.0f;

		Image ledim = Image(Image::ARGB, fbounds.getWidth() * 2.0f, fbounds.getHeight() * 2.0f, true);
		Graphics ledig(ledim);
		ledig.addTransform(juce::AffineTransform::translation(fbounds.getWidth() * 0.5f, fbounds.getHeight() * 0.5f));

		if (isLine) {
			glowradius = fbounds.getWidth() * 0.3f;		

			ledig.setColour(lcdPalette[offled].withAlpha(1.0f-alpha));
			ledig.fillRect(fbounds);
			ledig.setColour(lcdPalette[led].withAlpha(alpha));
			ledig.fillRect(fbounds);
		}
		else {
			glowradius = fbounds.getWidth() * 0.3f;

			ledig.setColour(lcdPalette[offled].withAlpha(1.0f - alpha));
			ledig.fillEllipse(fbounds);
			ledig.setColour(lcdPalette[led].withAlpha(alpha));
			ledig.fillEllipse(fbounds);
		}

		Image glowim = ledim.createCopy();
		Graphics glowig(glowim);

		GlowEffect glow;
		glow.setGlowProperties(glowradius * alpha, lcdPalette[led].withAlpha(alpha));
		glow.applyEffect(ledim, glowig, 1.0f, 1.0f);

		g.drawImage(glowim, fbounds);
	}

	void setBrightness(float a)
	{
		alpha = a;
	}

	bool isLineType() const
	{
		return isLine;
	}

private:
	float alpha = 0.0f;
	const bool isLine;
};

#endif // MOONJLED_H
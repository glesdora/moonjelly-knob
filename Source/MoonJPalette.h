#ifndef MOONJPALETTE_H
#define MOONJPALETTE_H

#pragma once

#include <juce_graphics/juce_graphics.h>

enum Palette { active, inactive, led, offled, grid, back, border, ledsband };

const juce::Colour lcdPalette[] = {
	juce::Colours::aliceblue,					// active
	juce::Colours::slategrey,					// inactive
	juce::Colours::aliceblue,					// led
	juce::Colours::darkslategrey,				// offled
	juce::Colours::grey.withAlpha(0.5F),		// grid
	juce::Colours::darkslategrey.darker(1.8F),	// back
	juce::Colours::darkslateblue			,	// border
	juce::Colours::transparentBlack				// ledsband
};

#endif // MOONJPALETTE_H
#pragma once

#include "SimpleWaveInterpolator.cpp"
#include "DoubleBuffer.h"
#include "MoonJKnob.h"

//==============================================================================
class WavetableOscillator
{
public:
    WavetableOscillator(const juce::AudioSampleBuffer& wavetableToUse)
        : wavetable(wavetableToUse)
    {
        jassert (wavetable.getNumChannels() == 1);
    }

    void setFrequency (float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = (float) wavetable.getNumSamples() / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }

    forcedinline float getNextSample() noexcept
    {
        auto tableSize = (unsigned int) wavetable.getNumSamples();

        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;

        auto frac = currentIndex - (float) index0;

        auto* table = wavetable.getReadPointer (0);
        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = value0 + frac * (value1 - value0);

        if ((currentIndex += tableDelta) > (float) tableSize)
            currentIndex -= (float) tableSize;

        return currentSample;
    }

private:
    const juce::AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f;
};

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent,
                               public juce::Timer,
	                           public juce::Slider::Listener 
{
public:
	MainContentComponent() : wi(tableSize), db(tableSize), knob("moonjelly")
    {
		auto& writeBuffer = db.getInactiveBuffer();
		wi.interpolate(0.0f, writeBuffer);
		db.swapBuffers();

        knob.setNumberOfShapes(wi.getNumWaveforms());
		knob.setData(db);
		addAndMakeVisible(knob);
		knob.addListener(this);

        createWavetable();

        setSize (400, 200);
        setAudioChannels (0, 2);
        startTimer (50);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
        knob.setBounds(getLocalBounds());
    }

    void timerCallback() override
    {
        auto cpu = deviceManager.getCpuUsage() * 100;
        cpuUsageText.setText (juce::String (cpu, 6) + " %", juce::dontSendNotification);
    }

	void sliderValueChanged(juce::Slider* slider) override
	{
		if (slider == &knob)
		{
			auto value = knob.getValue();
			if (value == knob.getMaximum())
				value = 0.0f;

			auto& writingBuffer = db.getInactiveBuffer();
			wi.interpolate(value, writingBuffer);
			db.swapBuffers();

            knob.updateShapePath();
        }
	}

	//--------------------------------------------------------------------------

    void createWavetable()
    {
        sineTable.setSize (1, (int) tableSize);
        auto* samples = sineTable.getWritePointer (0);

		const auto data = db.getActiveBuffer().data();

		sineTable.setDataToReferTo(db.getActiveBufferPointerArray(), 1, tableSize);

		for (unsigned int i = 0; i < tableSize; ++i)
		{
			samples[i] = db.getActiveBuffer()[i];
		}
    }

    void prepareToPlay (int, double sampleRate) override
    {
        auto numberOfOscillators = 4;

        for (auto i = 0; i < numberOfOscillators; ++i)
        {
            auto* oscillator = new WavetableOscillator (sineTable);

            auto midiNote = 12.0 * i + 36.0;
            auto frequency = 440.0 * pow (2.0, (midiNote - 69.0) / 12.0);

            oscillator->setFrequency ((float) frequency, (float) sampleRate);
            oscillators.add (oscillator);
        }

        level = 0.25f / (float) numberOfOscillators;
    }

    void releaseResources() override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

        bufferToFill.clearActiveBufferRegion();

        for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex)
        {
            auto* oscillator = oscillators.getUnchecked (oscillatorIndex);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                auto levelSample = oscillator->getNextSample() * level;

                leftBuffer[sample]  += levelSample;
                rightBuffer[sample] += levelSample;
            }
        }
    }

private:
    juce::Label cpuUsageLabel;
    juce::Label cpuUsageText;

    const unsigned int tableSize = 1 << 7;
    float level = 0.0f;

    juce::AudioSampleBuffer sineTable;
    juce::OwnedArray<WavetableOscillator> oscillators;

    SimpleWaveInterpolator wi;
    DoubleBuffer db;
	MoonJKnob knob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

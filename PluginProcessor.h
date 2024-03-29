/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//#define _ID ""
#define DELAYTIME_ID "delayTime"
#define DELAYTIME_NAME "DelayTime"

#define FEEDBACK_ID "feedback"
#define FEEDBACK_NAME "Feedback"
//==============================================================================
/**
*/
class DelayUnit2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayUnit2AudioProcessor();
    ~DelayUnit2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
	void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
	void feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer);

	double delayTime;
	double feedbackValue;
	AudioProcessorValueTreeState parameters;

private:
	AudioBuffer<float> mDelayBuffer; 
	int mWritePosition{ 0 };
	int mSampleRate{ 44100 };



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayUnit2AudioProcessor)
};

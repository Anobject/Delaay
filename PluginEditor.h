/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayUnit2AudioProcessorEditor  : public AudioProcessorEditor,
										public Slider::Listener
{
public:
    DelayUnit2AudioProcessorEditor (DelayUnit2AudioProcessor&);
    ~DelayUnit2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;
	
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> delayTimeAttach;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> FeedbackAttach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayUnit2AudioProcessor& processor;

	
	Slider sDelayTime;
	Label sDelayTimeLabel;

	Slider sFeedback;
	Label sFeedbackLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayUnit2AudioProcessorEditor)
};

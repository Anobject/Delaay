/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayUnit2AudioProcessorEditor::DelayUnit2AudioProcessorEditor (DelayUnit2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (720, 480);

	delayTimeAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, DELAYTIME_ID, sDelayTime);
	FeedbackAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, FEEDBACK_ID, sFeedback);
	//-------------------------------------------------------------sDelayTime--------------------------------------------------------------------
	addAndMakeVisible(sDelayTime);
	sDelayTime.setSliderStyle(Slider::SliderStyle::Rotary);
	sDelayTime.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
	sDelayTime.setRange(1.0, 2000.0);
	//sDelayTime.setValue(1);
	sDelayTime.setTextValueSuffix(" ms");
	sDelayTime.addListener(this);
	//sDelayTime.setNumDecimalPlacesToDisplay(1); 

	addAndMakeVisible(sDelayTimeLabel);
	sDelayTimeLabel.setText("Delay Time", dontSendNotification);
	sDelayTimeLabel.attachToComponent(&sDelayTime, false);
	//--------------------------------------------------------------sFeedback-------------------------------------------------------------------

	addAndMakeVisible(sFeedback);
	sFeedback.setSliderStyle(Slider::SliderStyle::Rotary);
	sFeedback.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
	sFeedback.setRange(1, 100);
	//sFeedback.setValue(50);
	sFeedback.setTextValueSuffix(" %");
	sFeedback.addListener(this);
	//sFeedback.setNumDecimalPlacesToDisplay(1);

	addAndMakeVisible(sFeedbackLabel);
	sFeedbackLabel.setText("Feedback", dontSendNotification);
	sFeedbackLabel.attachToComponent(&sFeedback, false);
	
}

DelayUnit2AudioProcessorEditor::~DelayUnit2AudioProcessorEditor()
{
}

//==============================================================================
void DelayUnit2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	g.fillAll(Colours::darkcyan);

	
}

void DelayUnit2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..


	sDelayTime.setBounds(((getWidth() / 2) * 1) - 200, (getHeight() / 2) - 100, 200, 200);
	sFeedback.setBounds(((getWidth() / 2) * 2) - 200, (getHeight() / 2) - 100, 200, 200);
	

	/*
	sDelayTime.setBounds(50, 125, 300, 300);
	sFeedback.setBounds(370, 125, 300, 300);
	*/
	
	
	/*
	const int border = 20;
	Rectangle<int> area = getLocalBounds();
	Rectangle<int> sliderArea = area.removeFromTop(area.getHeight() / 2);

	sDelayTime.setBounds(sliderArea.removeFromLeft(area.getWidth() / 2).reduced(border));
	sFeedback.setBounds(sliderArea.removeFromRight(area.getWidth() / 2).reduced(border));
	*/
}


void DelayUnit2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &sDelayTime)
	{
		processor.delayTime = sDelayTime.getValue();
	}

	if (slider == &sFeedback)
	{
		processor.feedbackValue = sFeedback.getValue();
	}


}



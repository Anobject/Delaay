/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayUnit2AudioProcessor::DelayUnit2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
		#if ! JucePlugin_IsMidiEffect
			#if ! JucePlugin_IsSynth
				.withInput("Input", AudioChannelSet::stereo(), true)
			#endif
				.withOutput("Output", AudioChannelSet::stereo(), true)
		#endif
				),

	delayTime(1),
	parameters(*this, nullptr)
#endif
{
	NormalisableRange<float> delayTimeRange(1.0, 2000.0);
	NormalisableRange<float> feedbackRange(1.0, 100.0);

	parameters.createAndAddParameter(DELAYTIME_ID, DELAYTIME_NAME, DELAYTIME_NAME, delayTimeRange, 100.0, nullptr, nullptr);
	parameters.createAndAddParameter(FEEDBACK_ID, FEEDBACK_NAME, FEEDBACK_NAME, feedbackRange, 20.0, nullptr, nullptr);
	parameters.state = ValueTree("savedParams");
}


DelayUnit2AudioProcessor::~DelayUnit2AudioProcessor()
{
}

//==============================================================================
const String DelayUnit2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayUnit2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayUnit2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayUnit2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayUnit2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayUnit2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayUnit2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayUnit2AudioProcessor::setCurrentProgram (int index)
{
}

const String DelayUnit2AudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayUnit2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayUnit2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	const int numInputChannels = getTotalNumInputChannels();
	const int delayBufferSize = 2 * (sampleRate * samplesPerBlock);
	mSampleRate = sampleRate;

	mDelayBuffer.setSize(numInputChannels, delayBufferSize);
}

void DelayUnit2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayUnit2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayUnit2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = mDelayBuffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
		float* dryBuffer = buffer.getWritePointer(channel);


		fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
		getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
		feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);

    }

	mWritePosition += bufferLength;
	mWritePosition %= delayBufferLength;
}

void DelayUnit2AudioProcessor::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
	const float gain = 0.9;


	if (delayBufferLength > bufferLength + mWritePosition)
	{
	
		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - mWritePosition;

		mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
		mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
	}
}

void DelayUnit2AudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
	int DelayTime = int (delayTime);
	const int readPosition = static_cast<int> (delayBufferLength + mWritePosition - (mSampleRate * DelayTime / 1000)) % delayBufferLength;

	if (delayBufferLength > bufferLength + readPosition)
	{
		buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - readPosition;
		buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
		buffer.copyFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
	}
}

void DelayUnit2AudioProcessor::feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer)
{
	const float rampGain = float (feedbackValue) / 100;

	if (delayBufferLength > bufferLength + mWritePosition)
	{
		mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, rampGain, rampGain);
	}
	else
	{
		const int bufferRemaining = delayBufferLength - mWritePosition;

		mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, rampGain, rampGain);
		mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, rampGain, rampGain);

	}
}

//==============================================================================
bool DelayUnit2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayUnit2AudioProcessor::createEditor()
{
    return new DelayUnit2AudioProcessorEditor (*this);
}

//==============================================================================
void DelayUnit2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	auto xml (parameters.state.createXml());
	copyXmlToBinary(*xml, destData);

}

void DelayUnit2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	auto theParams(getXmlFromBinary(data, sizeInBytes));

	if (theParams != nullptr);
	{
		if (theParams->hasTagName(parameters.state.getType()))
		{
			parameters.state = ValueTree::fromXml(*theParams);
		}
	}

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayUnit2AudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Boost1AudioProcessor::Boost1AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       )
{
}

Boost1AudioProcessor::~Boost1AudioProcessor()
{
}

//==============================================================================
const juce::String Boost1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Boost1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Boost1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Boost1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Boost1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Boost1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Boost1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Boost1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Boost1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Boost1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Boost1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Boost1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Boost1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void Boost1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    float linearGain = std::pow(10.0f, gainVal / 20.0f);

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] *= linearGain;
            }
        }
    
    const float* channelData = buffer.getReadPointer(0); // Get the first channel
        float sumSquared = 0.0f;

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            sumSquared += channelData[i] * channelData[i];
        }

    float rms = std::sqrt(sumSquared / buffer.getNumSamples());
    float db = juce::Decibels::gainToDecibels(rms, -100.0f); // Convert to dB
    float normalizedVolume = juce::jmap(db, -30.0f, 30.0f, 0.0f, 1.0f); // Map -100 dB to 0, 0 dB to 1
    normalizedVolume = juce::jlimit(0.0f, 1.0f, normalizedVolume);

    volumeLevel = normalizedVolume;

}

//==============================================================================
bool Boost1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Boost1AudioProcessor::createEditor()
{
    return new Boost1AudioProcessorEditor (*this);
}

//==============================================================================
void Boost1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Boost1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Boost1AudioProcessor();
}

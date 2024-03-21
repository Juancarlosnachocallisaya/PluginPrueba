/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
    juancarlos

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginPruebaAudioProcessor::PluginPruebaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts (*this,nullptr,"PARAMETERS",createparameters())
#endif
{
}

PluginPruebaAudioProcessor::~PluginPruebaAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginPruebaAudioProcessor::createparameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Gain",1 }, "Gain", -60.0f,60.0f,0.0f));

    return params;
}

//==============================================================================
const juce::String PluginPruebaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginPruebaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginPruebaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginPruebaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginPruebaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginPruebaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginPruebaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginPruebaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginPruebaAudioProcessor::getProgramName (int index)
{
    return {};
}

void PluginPruebaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginPruebaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    Gain.prepare(spec);
}

void PluginPruebaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginPruebaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PluginPruebaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    levelMeterINleft2 = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    levelMeterINright2 = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    

   
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

      levelMeterINleft =juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
      levelMeterINright =juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));

      auto audioblock = juce::dsp::AudioBlock<float>(buffer);
      auto context = juce::dsp::ProcessContextReplacing<float>(audioblock);

      Gain.setGainDecibels(apvts.getRawParameterValue("Gain")->load());
      
      Gain.process(context);

    }
}

//==============================================================================
bool PluginPruebaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginPruebaAudioProcessor::createEditor()
{
    return new PluginPruebaAudioProcessorEditor (*this);
}

//==============================================================================
void PluginPruebaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PluginPruebaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float PluginPruebaAudioProcessor::getRmsValue(const int channel) const

{

    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return levelMeterINleft;
    if (channel == 1)
        return levelMeterINright;
}

float PluginPruebaAudioProcessor::getRmsValue2(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return levelMeterINleft2;
    if (channel == 1)
        return levelMeterINright2;

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginPruebaAudioProcessor();
}

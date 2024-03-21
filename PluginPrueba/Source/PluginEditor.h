/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Medidor/Medidor.h"

//==============================================================================
/**
*/
class PluginPruebaAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    PluginPruebaAudioProcessorEditor (PluginPruebaAudioProcessor&);
    ~PluginPruebaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    
    LevelMeter2 MedidorOUTL, MedidorOUTR;

    LevelMeter2 MedidorINL, MedidorINR;

    PluginPruebaAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginPruebaAudioProcessorEditor)
};

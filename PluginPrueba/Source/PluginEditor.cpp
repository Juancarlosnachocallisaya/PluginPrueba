/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginPruebaAudioProcessorEditor::PluginPruebaAudioProcessorEditor (PluginPruebaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(MedidorOUTL);
    addAndMakeVisible(MedidorOUTR);

    addAndMakeVisible(MedidorINL);
    addAndMakeVisible(MedidorINR);



    setSize (400, 300);
    startTimerHz(25);
}

PluginPruebaAudioProcessorEditor::~PluginPruebaAudioProcessorEditor()
{
}

void PluginPruebaAudioProcessorEditor::timerCallback()

{
    MedidorINL.setlevel(audioProcessor.getRmsValue(0));
    MedidorINR.setlevel(audioProcessor.getRmsValue(1));


    MedidorINL.repaint();
    MedidorINR.repaint();



    MedidorOUTL.setlevel(audioProcessor.getRmsValue(0));
    MedidorOUTR.setlevel(audioProcessor.getRmsValue(1));

    MedidorOUTL.repaint();
    MedidorOUTR.repaint();
    
}


//==============================================================================
void PluginPruebaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PluginPruebaAudioProcessorEditor::resized()
{
    MedidorOUTL.setBounds(250, 100, 150, 15);
    MedidorOUTR.setBounds(250, 135, 150, 15);

 

   MedidorINL.setBounds(100, 100, 150, 15);
   MedidorINR.setBounds(100, 135, 150, 15);
}

/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
    FRONTEND
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

//CONSTRUCTOR
MusicMagicAudioProcessorEditor::MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30); //pos, read only, w, h
    RandomnessSlider.setRange(0, 100);
    RandomnessSlider.setNumDecimalPlacesToDisplay(0);
    RandomnessSlider.setValue(50);
    addAndMakeVisible(RandomnessSlider);
    
    
    
    //size of window
    setSize (500, 600);
}

MusicMagicAudioProcessorEditor::~MusicMagicAudioProcessorEditor()
{
}

//==============================================================================
void MusicMagicAudioProcessorEditor::paint (juce::Graphics& g)
{
    //background colour
    g.fillAll (juce::Colour(0x21, 0x21, 0x21));

    //text styling
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MusicMagicAudioProcessorEditor::resized()
{   //where subcomponents are laid
    RandomnessSlider.setBounds(275, 100, 150, 150); //x, y, h, w
}

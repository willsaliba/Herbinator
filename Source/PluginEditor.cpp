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
    g.fillAll(juce::Colour(0x21, 0x21, 0x21));
        
    //input square
    g.setColour(juce::Colours::black); //inner
    g.fillRect(10, 10, 480, 80);
    g.setColour(juce::Colour(25, 160, 250)); //border
    g.drawRect(juce::Rectangle<int>(10, 10, 480, 80), 2.0f);
    
    //action square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 100, 300, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 100, 300, 220), 2.0f);
    
    //randomiser square
    g.setColour(juce::Colours::black);
    g.fillRect(320, 100, 170, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(320, 100, 170, 220), 2.0f);
    
    //prompt square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 330, 480, 80);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 330, 480, 80), 2.0f);
    
    //output square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 510, 480, 80);
    g.setColour(juce::Colour(190, 25, 250));
    g.drawRect(juce::Rectangle<int>(10, 510, 480, 80), 2.0f);
    
    //text for each box
    juce::Font myFont(20.0f);
    myFont = myFont.boldened();
    g.setFont(myFont);
    g.setColour(juce::Colours::white);
    g.drawFittedText("Input Track", 25, 25, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Action", 110, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Randomiser", 355, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Prompt", 25, 345, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Output Track", 25, 525, 150, 50, juce::Justification::left, false);
         
}

void MusicMagicAudioProcessorEditor::resized()
{   //where subcomponents are laid
    RandomnessSlider.setBounds(330, 150, 150, 150); //x, y, h, w
}

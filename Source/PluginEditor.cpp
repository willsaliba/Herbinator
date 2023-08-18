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
{   //initialisomg components
    
    //input
    input_play_button.setButtonText("P");
    input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(input_play_button);

    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30); //pos, read only, w, h
    RandomnessSlider.setRange(0, 100);
    RandomnessSlider.setNumDecimalPlacesToDisplay(0);
    RandomnessSlider.setValue(50);
    addAndMakeVisible(RandomnessSlider);
    
    //ACTION BUTTONS
    extendButton.setButtonText("Extend");
    addAndMakeVisible(extendButton);
    fillButton.setButtonText("Fill");
    addAndMakeVisible(fillButton);
    replaceButton.setButtonText("Replace");
    addAndMakeVisible(replaceButton);
    generateButton.setButtonText("Generate");
    addAndMakeVisible(generateButton);
    
    //PROMPT
    userPrompt.setMultiLine(false);
    userPrompt.setReturnKeyStartsNewLine(false);
    userPrompt.setReadOnly(false);
    userPrompt.setText("");
    userPrompt.setJustification(32);
    userPrompt.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    userPrompt.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    addAndMakeVisible(userPrompt);
    
    //generate music button
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    addAndMakeVisible(generate_music_button);

    //output
    output_play_button.setButtonText("P");
    output_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(output_play_button);
    output_copy_button.setButtonText("C");
    output_copy_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(output_copy_button);
    
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
    
    //action square buttons
    g.setColour(juce::Colour(85, 85, 85));
    g.fillRect(25, 160, 127.5, 50);
    g.fillRect(167.5, 160, 127.5, 50);
    g.fillRect(25, 235, 127.5, 50);
    g.fillRect(167.5, 235, 127.5, 50);
    
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
{   //psoitioning components
    
    //input
    input_play_button.setBounds(440, 35, 30, 30);
    
    //randomness
    RandomnessSlider.setBounds(330, 150, 150, 150); //x, y, w, h
    
    //action buttons
    extendButton.setBounds(35, 175, 100, 20);
    fillButton.setBounds(35, 250, 100, 20);
    replaceButton.setBounds(177.5, 175, 100, 20);
    generateButton.setBounds(177.5, 250, 100, 20);
    
    //prompt
    userPrompt.setBounds(105, 355, 360, 30);
    
    //generate music button
    generate_music_button.setBounds(125, 435, 250, 50);
    
    //output
    output_play_button.setBounds(400, 535, 30, 30);
    output_copy_button.setBounds(440, 535, 30, 30);
}

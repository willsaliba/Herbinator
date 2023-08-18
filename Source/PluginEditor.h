/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
    --- THIS IS FRONT END ---
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class MusicMagicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor&);
    ~MusicMagicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //input
    juce::TextButton input_play_button;
    
    //randomness
    juce::Slider RandomnessSlider;
    
    //actions
    juce::ToggleButton extendButton;
    juce::ToggleButton fillButton;
    juce::ToggleButton replaceButton;
    juce::ToggleButton generateButton;
    
    //prompt
    juce::TextEditor userPrompt;
    
    //generate button
    juce::TextButton generate_music_button;
    
    //output
    juce::TextButton output_play_button;
    juce::TextButton output_copy_button;
    
    
    
    // reference provided as quick way for editor to access processor object that created it.
    MusicMagicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessorEditor)
};

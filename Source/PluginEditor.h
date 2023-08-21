/* JUCE Plugin Editor --- FRONT END --- */

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================

class MusicMagicAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget,
                                        private juce::Timer
{
public:
    //constructor & destructor
    MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor&);
    ~MusicMagicAudioProcessorEditor() override;
    
    //building window
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //general functionality
    void toggleOn(juce::ToggleButton& onButton);
    
    //file drag&drop
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    void updateInputTrackDesign();
    
    //generate request
    void generate_request();
    bool check_valid_request();
    void send_request(juce::String prompt, juce::String action, juce::String randomness);

private:
    
    //input
    juce::TextButton input_load_box;
    juce::TextButton input_play_button;
    juce::TextButton input_delete_button;
    
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
    void timerCallback() override;
    
    //output
    juce::TextButton output_play_button;
    juce::TextButton output_copy_button;
    
    
    //in final \X/
    juce::TextButton promptBut;
    juce::TextButton randBut;
    juce::TextButton actionBut;
    
    
    // reference provided as quick way for editor to access processor object that created it.
    MusicMagicAudioProcessor& MusMagProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessorEditor)
};

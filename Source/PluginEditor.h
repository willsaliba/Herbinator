/* JUCE Plugin Editor --- FRONT END --- */

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class MusicMagicAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        private juce::Timer,
                                        public juce::FileDragAndDropTarget,
                                        public juce::DragAndDropContainer
{
public:
    
    //constructor & destructor
    MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor&);
    ~MusicMagicAudioProcessorEditor() override;
    
    //building window
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //drag and drop files in:
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    
private:
    
    //input
    juce::TextButton input_load_box;
    juce::TextButton input_play_button;
    juce::TextButton input_stop_button;
    juce::TextButton input_delete_button;
    void initialiseInputComponents();
    void updateInputTrackDesign();
    
    //randomness
    juce::Slider RandomnessSlider;
    
    //actions
    juce::ToggleButton extendButton;
    juce::ToggleButton fillButton;
    juce::ToggleButton replaceButton;
    juce::ToggleButton generateButton;
    void toggleOn(juce::ToggleButton& onButton, juce::String action);
    juce::TextButton input_cover;
    
    //prompt
    juce::TextEditor userPrompt;
    
    //generate button
    juce::TextButton generate_music_button;
    void timerCallback() override;
    void ui_update_invalid_request();
    void ui_update_request_sent();
    void generate_request();
    
    //output
    juce::TextButton output_track_box;
    juce::TextButton output_play_button;
    juce::TextButton output_stop_button;
    juce::TextButton output_delete_button;
    void initialiseOutputComponents();
    void mouseDrag(const juce::MouseEvent& event) override;
    void updateOutputTrackDesign();
    
    //XXX
    juce::TextButton selectOutput;
    
    //reference to call processor functions
    MusicMagicAudioProcessor& MusMagProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessorEditor)
};

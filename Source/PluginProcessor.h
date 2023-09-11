/* JUCE Plugin Processor --- BACK END --- */

#pragma once
#include <JuceHeader.h>

class MusicMagicAudioProcessor  : public juce::AudioProcessor
                                #if JucePlugin_Enable_ARA
                                 , public juce::AudioProcessorARAExtension
                                #endif
{
public:
    
    MusicMagicAudioProcessor();
    ~MusicMagicAudioProcessor() override;
    
    //==============================================================PREDEFINED & EDITED
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //============================================================PREDEFINED & UNEDITED
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; };
    const juce::String getName() const override { return JucePlugin_Name;};
    bool acceptsMidi() const override { return true; };
    bool producesMidi() const override { return true; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override { return 0.0; };
    int getNumPrograms() override { return 1; };
    int getCurrentProgram() override { return 0; };
    void setCurrentProgram (int index) override {};
    const juce::String getProgramName (int index) override { return {}; };
    void changeProgramName (int index, const juce::String& newName) override {};
    void getStateInformation (juce::MemoryBlock& destData) override {};
    void setStateInformation (const void* data, int sizeInBytes) override {};
    void releaseResources() override {};
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    //==============================================================================
    
    void demonstration();
    
    //IO selection and playing sound
    bool inputSelected;
    void setInput(bool isInput) { inputSelected = isInput; };
    void sendMIDInotes();
    void sendNoSound();
    
    //Input
    juce::File inputTrack;
    juce::String inputTrackPath;
    void loadInputFile();
    void loadInputFile(const juce::String& path);
    int getNumSamplerSounds() { return mSampler.getNumSounds(); };
    void clearInputSampler();
    
    //Output
    juce::File outputTrack;
    juce::String pathToClip;
    int getNumOutputSounds() { return outputSampler.getNumSounds(); };
    void clearOutputSampler();
    juce::String getPath() { return pathToClip; };
    
    //making request
    bool process_request(juce::String prompt, juce::String action, juce::String random);
    bool send_request_to_model(juce::String prompt, juce::String action, juce::String random);
    
private:
    
    //Input
    juce::Synthesiser mSampler;
    const int mNumVoices { 1 };
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader { nullptr };
    
    //Output
    juce::Synthesiser outputSampler;
    const int outputNumVoices { 1 };
    juce::AudioFormatManager outputFormatManager;
    juce::AudioFormatReader* outputFormatReader { nullptr };
    
    //generate request functions xxx
    bool valid_extend_request(juce::String prompt) {return false;}; //prmp, path, side
    bool valid_fill_request(juce::String prompt) {return false;};
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessor)
};

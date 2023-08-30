/* JUCE Plugin Processor --- BACK END --- */

#pragma once
#include <JuceHeader.h>

class MusicMagicAudioProcessor  : public juce::AudioProcessor
                                #if JucePlugin_Enable_ARA
                                 , public juce::AudioProcessorARAExtension
                                #endif
{
public:
    
    //constructor & destructor
    MusicMagicAudioProcessor();
    ~MusicMagicAudioProcessor() override;
    
    //=====================================================================PREDEFINED
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; };
    const juce::String getName() const override;
    bool acceptsMidi() const override { return true; };
    bool producesMidi() const override { return true; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override { return 0.0; };
    int getNumPrograms() override { return 1; };
    int getCurrentProgram() override { return 0; };
    //======================================================================PREDEFINED
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    #ifndef JucePlugin_PreferredChannelConfigurations
      bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //==============================================================================
    //=============================================================================
    
    bool inputSelected;
    
    
    //======================================================================Input
    juce::File inputTrack;
    void loadInputFile();
    void loadInputFile(const juce::String& path);

    int getNumSamplerSounds() { return mSampler.getNumSounds(); };
    void clearInputSampler();
    
    //======================================================================Output
    juce::File outputTrack;
    void loadOutputFile();
    int getNumOutputSounds() { return outputSampler.getNumSounds(); };
    void clearOutputSampler();
    
private:
    
    //======================================================================Input
    juce::Synthesiser mSampler;
    const int mNumVoices { 1 };
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader { nullptr };
    
    //======================================================================Output
    juce::Synthesiser outputSampler;
    const int outputNumVoices { 1 };
    juce::AudioFormatManager outputFormatManager;
    juce::AudioFormatReader* outputFormatReader { nullptr };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessor)
};

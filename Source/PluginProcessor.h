/* JUCE Plugin Processor --- BACK END --- */

#pragma once
#include <JuceHeader.h>

//==============================================================================

class MusicMagicAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    
    //constructor & destructor
    MusicMagicAudioProcessor();
    ~MusicMagicAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; };
    const juce::String getName() const override;
    bool acceptsMidi() const override { return true; };
    bool producesMidi() const override { return true; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override { return 0.0; };
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    
    //custom methods:
    void loadFile(const juce::String& path);
    int getNumSamplerSounds() { return mSampler.getNumSounds(); }
    void clearInputTrack();
    void playSample();

private:
    
    //variables for making sampler
    juce::Synthesiser mSampler;
    const int numVoices {1};
    
    //format manager for sampler
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader {nullptr};
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicMagicAudioProcessor)
};

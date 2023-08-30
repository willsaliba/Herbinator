#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

MusicMagicAudioProcessor::MusicMagicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    inputSelected = true;
    
    //input
    mFormatManager.registerBasicFormats();
    mSampler.addVoice(new juce::SamplerVoice());
    
    //output
    outputFormatManager.registerBasicFormats();
    outputSampler.addVoice(new juce::SamplerVoice());
}

MusicMagicAudioProcessor::~MusicMagicAudioProcessor()
{
    //clearing samplers
    clearInputSampler();
    clearOutputSampler();
}

//============================================================================== PREDEFINED BUT HAS IMPLEMENTATION

void MusicMagicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //parses through midi for us
    if (inputSelected) mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    else outputSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void MusicMagicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    outputSampler.setCurrentPlaybackSampleRate(sampleRate);
}

//============================================================================== INPUT TRACK

void MusicMagicAudioProcessor::loadInputFile()
{
    mSampler.clearSounds();
    
    //selecting file from directory
    juce::FileChooser chooser { "Please load a file" };
    
    //if sound chosen
    if ( chooser.browseForFileToOpen() ) {
        inputTrack = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(inputTrack);
        
        //adding sound to Sampler
        juce::BigInteger range;
        range.setRange(0, 128, true);
        mSampler.addSound( new juce::SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
    }
}

void MusicMagicAudioProcessor::loadInputFile(const juce::String& path)
{
    mSampler.clearSounds();
    
    //path retrieved by UI
    inputTrack = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(inputTrack);
    
    //adding sound to Sampler
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound( new juce::SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

void MusicMagicAudioProcessor::clearInputSampler()
{
    mSampler.clearSounds();
    if (mFormatReader) {
        delete mFormatReader;
        mFormatReader = nullptr;
    }
}

//============================================================================== INPUT TRACK

void MusicMagicAudioProcessor::loadOutputFile()
{
    outputSampler.clearSounds();
    
    //selecting file from directory
    juce::FileChooser chooser { "Please load a file" };
    
    //if sound chosen
    if ( chooser.browseForFileToOpen() ) {
        outputTrack = chooser.getResult();
        outputFormatReader = outputFormatManager.createReaderFor(outputTrack);
        
        //adding sound to Sampler
        juce::BigInteger range;
        range.setRange(0, 128, true);
        outputSampler.addSound( new juce::SamplerSound ("Sample", *outputFormatReader, range, 60, 0.1, 0.1, 10.0));
    }
}

void MusicMagicAudioProcessor::clearOutputSampler()
{
    outputSampler.clearSounds();
    if (outputFormatReader) {
        delete outputFormatReader;
        outputFormatReader = nullptr;
    }
}

//============================================================================== BLAH






















//============================================================================== UNTOUCHED
//==============================================================================

const juce::String MusicMagicAudioProcessor::getName() const
{ return JucePlugin_Name;}
void MusicMagicAudioProcessor::setCurrentProgram (int index)
{ }
const juce::String MusicMagicAudioProcessor::getProgramName (int index)
{ return {}; }
void MusicMagicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{ }
void MusicMagicAudioProcessor::releaseResources()
{ }
void MusicMagicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{ }
void MusicMagicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{ }

#ifndef JucePlugin_PreferredChannelConfigurations
bool MusicMagicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ return new MusicMagicAudioProcessor(); }

// This creates new instance of the editor (front end)
juce::AudioProcessorEditor* MusicMagicAudioProcessor::createEditor()
{ return new MusicMagicAudioProcessorEditor (*this); }

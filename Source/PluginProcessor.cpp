#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================

//CONSTRUCTOR
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
    //setting up sampler
    mFormatManager.registerBasicFormats();
    mSampler.addVoice( new juce::SamplerVoice() );
    
}

//DESTRUCTOR
MusicMagicAudioProcessor::~MusicMagicAudioProcessor() {
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String MusicMagicAudioProcessor::getName() const {
    return JucePlugin_Name;
}
//==============================================================================

int MusicMagicAudioProcessor::getNumPrograms() { return 1;  } // min 1, even if not implementing programs
int MusicMagicAudioProcessor::getCurrentProgram() { return 0; }
void MusicMagicAudioProcessor::setCurrentProgram (int index) {}
const juce::String MusicMagicAudioProcessor::getProgramName (int index) { return {}; }
void MusicMagicAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

//==============================================================================
void MusicMagicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

void MusicMagicAudioProcessor::releaseResources()
{  //free up memory when playback stops
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MusicMagicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MusicMagicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (may not be empty).
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //for sampler
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================

juce::AudioProcessorEditor* MusicMagicAudioProcessor::createEditor() {
    return new MusicMagicAudioProcessorEditor (*this);
}

//==============================================================================
void MusicMagicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{ /* method to store your parameters in the memory block */ }

void MusicMagicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{ /* restore your parameters from this memory block (contents created by getStateInformation() call) */ }
//============================================================================== ADDED METHODS

//loading dropped file
void MusicMagicAudioProcessor::loadFile(const juce::String& path) {
    
    mSampler.clearSounds();
    auto file = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(file);
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
    
}

void  MusicMagicAudioProcessor::clearInputTrack() { mSampler.clearSounds(); }

void MusicMagicAudioProcessor::playSample() {
    
}


//==============================================================================

//creating instances of plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new MusicMagicAudioProcessor();
}

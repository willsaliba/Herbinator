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
    mFormatManager.registerBasicFormats();
    mSampler.addVoice(new juce::SamplerVoice());
}

MusicMagicAudioProcessor::~MusicMagicAudioProcessor()
{
    mFormatReader = nullptr;
}

//============================================================================== UNTOUCHED

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

//============================================================================== PREDEFINED BUT HAS IMPLEMENTATION

//UNTOUCHED
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

//TOUCHED
void MusicMagicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //parses through midi for us
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//TOUCHED
void MusicMagicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

//============================================================================== CUSTOM FUNCTIONALITY

void MusicMagicAudioProcessor::loadFile()
{
    //selecting file from directory
    juce::FileChooser chooser { "Please load a file" };
    if ( chooser.browseForFileToOpen() ) {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }

    //adding sound to Sampler
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound( new juce::SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));

}


void MusicMagicAudioProcessor::playButtonClicked() {}
void MusicMagicAudioProcessor::stopButtonClicked() {}

//==============================================================================

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ return new MusicMagicAudioProcessor(); }

// This creates new instance of the editor (front end)
juce::AudioProcessorEditor* MusicMagicAudioProcessor::createEditor()
{ return new MusicMagicAudioProcessorEditor (*this); }

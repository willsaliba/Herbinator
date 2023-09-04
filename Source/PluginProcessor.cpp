#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

//CONSTRUCTOR
MusicMagicAudioProcessor::MusicMagicAudioProcessor() : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    ),
    inputSelected(true), pathToClip("")
{
    //input
    mFormatManager.registerBasicFormats();
    mSampler.addVoice(new juce::SamplerVoice());
    
    //output
    outputFormatManager.registerBasicFormats();
    outputSampler.addVoice(new juce::SamplerVoice());
    
}

//DESTRUCTOR
MusicMagicAudioProcessor::~MusicMagicAudioProcessor()
{
    clearInputSampler();
    clearOutputSampler();
}

//================================================================= PLAYING SOUND

//predefined
void MusicMagicAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

//predefined
void MusicMagicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    outputSampler.setCurrentPlaybackSampleRate(sampleRate);
}

//sends MIDI notes to play selected song
void MusicMagicAudioProcessor::sendMIDInotes()
{
    juce::MidiBuffer midiMessages;
    midiMessages.addEvent(juce::MidiMessage::noteOn(1, 60, (juce::uint8)127), 0);
    juce::AudioBuffer<float> tempBuffer;
    processBlock(tempBuffer, midiMessages);
}

//sends 0 freq MIDI to play no sound
void MusicMagicAudioProcessor::sendNoSound()
{
    juce::MidiBuffer midiMessages;
    midiMessages.addEvent(juce::MidiMessage::noteOff(1, 60), 0);
    juce::AudioBuffer<float> tempBuffer;
    processBlock(tempBuffer, midiMessages);
}

//============================================================================== INPUT TRACK

void MusicMagicAudioProcessor::loadInputFile()
{
    mSampler.clearSounds();
    
    //selecting file from directory
    juce::FileChooser chooser { "Please load an Input file" };
    
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

//============================================================================== Output TRACK

void MusicMagicAudioProcessor::loadOutputFile()
{
    outputSampler.clearSounds();
    
    //selecting file from directory
    juce::FileChooser chooser { "Please load an Output file" };
    
    //if sound chosen
    if ( chooser.browseForFileToOpen() ) {
        
        outputTrack = chooser.getResult();
        outputFormatReader = outputFormatManager.createReaderFor(outputTrack);
        pathToClip = outputTrack.getFullPathName();
        
        //adding sound to Sampler
        juce::BigInteger range;
        range.setRange(0, 128, true);
        outputSampler.addSound( new juce::SamplerSound ("Sample", *outputFormatReader, range, 60, 0.1, 0.1, 10.0));
    }
}

void MusicMagicAudioProcessor::clearOutputSampler()
{
    pathToClip = "";
    outputSampler.clearSounds();
    if (outputFormatReader) {
        delete outputFormatReader;
        outputFormatReader = nullptr;
    }
}

//============================================================================== Making Request

bool MusicMagicAudioProcessor::process_request(juce::String prompt, juce::String action, juce::String random)
{
    //checking action selected
    if (action == "Unselected") return false;
    //checking if selected action has appropriate
    bool valid = false;
    if (action == "Generate") valid = valid_generate_request(prompt);
    else if (action == "Replace") valid = valid_replace_request(prompt);
    else if (action == "Fill") valid = valid_fill_request(prompt);
    else if (action == "Extend") valid = valid_extend_request(prompt);
    //if valid request send it to the model
    if (valid) return send_request_to_model(prompt, action, random);
    else return false;
}

bool MusicMagicAudioProcessor::valid_generate_request(juce::String prompt) {
    if (prompt != "") return true;
    return false;
}

bool MusicMagicAudioProcessor::valid_replace_request(juce::String prompt) {
    if (prompt != "") return true; //& filepath
    return false;
}

bool MusicMagicAudioProcessor::valid_extend_request(juce::String prompt) {
    if (prompt != "") return true; //& filepath & time & side
    return false;
}

bool MusicMagicAudioProcessor::valid_fill_request(juce::String prompt) {
    if (prompt != "") return true; //& filepath & clip1 & clip2 etc
    return false;
}

bool MusicMagicAudioProcessor::send_request_to_model(juce::String prompt, juce::String action, juce::String random)
{
    return true;
}

//============================================================================== NEW



//============================================================================== UNTOUCHED

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

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ return new MusicMagicAudioProcessor(); } //new plugin instance

juce::AudioProcessorEditor* MusicMagicAudioProcessor::createEditor()
{ return new MusicMagicAudioProcessorEditor (*this); } //new editor instance

//==============================================================================

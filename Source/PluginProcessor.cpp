#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

//CONSTRUCTOR
MusicMagicAudioProcessor::MusicMagicAudioProcessor() : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    ),
    inputSelected(true), inputTrackPath(""), pathToClip("")
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
        inputTrackPath = inputTrack.getFullPathName();
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
    inputTrackPath = path;
    inputTrack = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(inputTrack);
    
    //adding sound to Sampler
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound( new juce::SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

void MusicMagicAudioProcessor::clearInputSampler()
{
    inputTrackPath = "";
    mSampler.clearSounds();
    if (mFormatReader) {
        delete mFormatReader;
        mFormatReader = nullptr;
    }
}

//============================================================================== Output TRACK

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
    //checking if selected action has appropriate arguments
    bool valid = false;
    if (action == "Generate") {
        if (prompt != "") valid = true;
    } else if (action == "Replace") {
        if (prompt != "" && inputTrackPath != "") valid = true;
    } else if (action == "Fill") {
        valid = valid_fill_request(prompt);
    } else if (action == "Extend") {
        valid = valid_extend_request(prompt);
    }
    //if valid request send it to the model
    if (valid) return true;
    else return false;
}

bool MusicMagicAudioProcessor::send_request_to_model(juce::String prompt, juce::String action, juce::String random)
{
    //converting args to std::string
    std::string thePrompt = prompt.toStdString();
    std::string theAction = action.toStdString();
    std::string theRandom = random.toStdString();
    std::string thePath = inputTrackPath.toStdString();
    
    //### hardcoded filepath
    std::string condaPythonPath = "/Users/willsaliba/opt/anaconda3/envs/riffusion/bin/python3";
    std::string modelPath = "/Users/willsaliba/Documents/Topics/diffusion-music";
    std::string cmd = condaPythonPath + " " + modelPath +  "/plugin_requests.py \"" + thePrompt + "\" \"" + theAction
        + "\" " + theRandom + " \"" + thePath + "\"";
    
    //juce run command process
    juce::ChildProcess childProcess;
    if (! childProcess.start(juce::String(cmd)) ) {
       DBG("Failed to start the process.");
       return false;
    }
    
    //capture output from model
    juce::String result = childProcess.readAllProcessOutput();
    result = result.removeCharacters(" \n\r");
    DBG("---"); DBG(result); DBG("---");

    //if success
    if ( !result.isEmpty() ) {
        outputSampler.clearSounds();
        //processing path file
        pathToClip = result;
        outputTrack = juce::File(pathToClip);
        outputFormatReader = outputFormatManager.createReaderFor(outputTrack);
        //adding sound to Sampler
        juce::BigInteger range;
        range.setRange(0, 128, true);
        outputSampler.addSound( new juce::SamplerSound ("Sample", *outputFormatReader, range, 60, 0.1, 0.1, 10.0));
        return true;
    }
    return false;
}





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

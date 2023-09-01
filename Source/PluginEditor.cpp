#include "PluginProcessor.h"
#include "PluginEditor.h"

//=============================================== con/de-structors + building page

//CONSTRUCTOR
MusicMagicAudioProcessorEditor::MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor& p)
    : AudioProcessorEditor (&p), MusMagProcessor (p)
{
    //INPUTPUT COMPONENTS
    initialiseInputComponents();
    
    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30);
    RandomnessSlider.setRange(0, 100);
    RandomnessSlider.setNumDecimalPlacesToDisplay(0);
    RandomnessSlider.setValue(50);
    addAndMakeVisible(RandomnessSlider);
    
    //ACTION BUTTONS
    extendButton.setButtonText("Extend");
    extendButton.onClick = [&]() { toggleOn(extendButton); };
    addAndMakeVisible(extendButton);
    fillButton.setButtonText("Fill");
    fillButton.onClick = [&]() { toggleOn(fillButton); };
    addAndMakeVisible(fillButton);
    replaceButton.setButtonText("Replace");
    replaceButton.onClick = [&]() { toggleOn(replaceButton); };
    addAndMakeVisible(replaceButton);
    generateButton.setButtonText("Generate");
    generateButton.onClick = [&]() { toggleOn(generateButton); };
    addAndMakeVisible(generateButton);
    
    //PROMPT
    userPrompt.setMultiLine(false);
    userPrompt.setReturnKeyStartsNewLine(false);
    userPrompt.setReadOnly(false);
    userPrompt.setText("");
    userPrompt.setJustification(32);
    userPrompt.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    userPrompt.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    addAndMakeVisible(userPrompt);
    
    //GENERATE MUSIC
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    generate_music_button.onClick = [&]() { generate_request(); };
    addAndMakeVisible(generate_music_button);

    //OUTPUT COMPONENTS
    initialiseOutputComponents();
    
    //XXX
    selectOutput.setButtonText("S");
    selectOutput.onClick = [&]() {
        MusMagProcessor.loadOutputFile();
        updateOutputTrackDesign();
    };
    addAndMakeVisible(selectOutput);
        
    //size of window
    setSize (500, 600);
}

//DESTRUCTOR
MusicMagicAudioProcessorEditor::~MusicMagicAudioProcessorEditor() {}

//non-functional graphics
void MusicMagicAudioProcessorEditor::paint(juce::Graphics& g)
{
    //background colour
    g.fillAll(juce::Colour(0x21, 0x21, 0x21));
        
    //input square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 10, 480, 80);
    g.setColour(juce::Colour(25, 160, 250));
    g.drawRect(juce::Rectangle<int>(10, 10, 480, 80), 2.0f);
    
    //action square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 100, 300, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 100, 300, 220), 2.0f);
    
    //action square buttons
    g.setColour(juce::Colour(85, 85, 85));
    g.fillRect(25, 160, 127.5, 50);
    g.fillRect(167.5, 160, 127.5, 50);
    g.fillRect(25, 235, 127.5, 50);
    g.fillRect(167.5, 235, 127.5, 50);
    
    //randomiser square
    g.setColour(juce::Colours::black);
    g.fillRect(320, 100, 170, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(320, 100, 170, 220), 2.0f);
    
    //prompt square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 330, 480, 80);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 330, 480, 80), 2.0f);
    
    //output square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 510, 480, 80);
    g.setColour(juce::Colour(190, 25, 250));
    g.drawRect(juce::Rectangle<int>(10, 510, 480, 80), 2.0f);
    
    //text for each box
    juce::Font myFont(20.0f);
    myFont = myFont.boldened();
    g.setFont(myFont);
    g.setColour(juce::Colours::white);
    g.drawFittedText("Output\nTrack", 60, 30, 65, 40, juce::Justification::left, false);
    g.drawFittedText("Action", 110, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Randomiser", 355, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Prompt", 25, 345, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Output\nTrack", 60, 530, 65, 40, juce::Justification::left, false);
    
}

//positioning of active elements
void MusicMagicAudioProcessorEditor::resized()
{
    //input
    inputFocusButton.setBounds(25, 20, 100, 60);
    input_load_box.setBounds(150, 20, 260, 60);
    input_delete_button.setBounds(435, 35, 30, 30);
    
    //randomness
    RandomnessSlider.setBounds(330, 150, 150, 150);
    
    //action buttons
    extendButton.setBounds(35, 175, 100, 20);
    fillButton.setBounds(35, 250, 100, 20);
    replaceButton.setBounds(177.5, 175, 100, 20);
    generateButton.setBounds(177.5, 250, 100, 20);
    
    //prompt
    userPrompt.setBounds(105, 355, 360, 30);
    
    //generate music button
    generate_music_button.setBounds(125, 435, 250, 50);
    
    //output
    outputFocusButton.setBounds(25, 520, 100, 60);
    output_track_box.setBounds(150, 520, 260, 60);
    output_delete_button.setBounds(435, 535, 30, 30);
    
    //XXX
    selectOutput.setBounds(435, 445, 30, 30);
}

//refactoring to declutter constructor
void MusicMagicAudioProcessorEditor::initialiseInputComponents()
{
    //input focus button
    toggleIO("Input");
    inputFocusButton.onClick = [&]() { toggleIO("Input"); };
    addAndMakeVisible(inputFocusButton);
    
    //load input track box
    input_load_box.setButtonText("Drag and Drop Or Click To Select");
    input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_load_box.onClick = [&]() {
        MusMagProcessor.loadInputFile();
        updateInputTrackDesign();
    };
    addAndMakeVisible(input_load_box);
    
    //delete input button
    input_delete_button.setButtonText("X");
    input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_delete_button.onClick = [&]() {
        MusMagProcessor.clearInputSampler();
        updateInputTrackDesign();
    };
    addAndMakeVisible(input_delete_button);
}

//refactoring to declutter constructor
void MusicMagicAudioProcessorEditor::initialiseOutputComponents()
{
    //output track box
    output_track_box.setButtonText("Currently No Output");
    output_track_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_track_box.addMouseListener(this, false);
    addAndMakeVisible(output_track_box);
    
    //output delete button
    output_delete_button.setButtonText("X");
    output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_delete_button.onClick = [&]() {
        MusMagProcessor.clearOutputSampler();
        updateOutputTrackDesign();
    };
    addAndMakeVisible(output_delete_button);
    
    //output focus button
    outputFocusButton.onClick = [&]() { toggleIO("Output"); };
    addAndMakeVisible(outputFocusButton);
    
}

//========================================================================= GENERAL
//ensuring only 1 action displays on
void MusicMagicAudioProcessorEditor::toggleOn(juce::ToggleButton& onButton)
{
    extendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    fillButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    generateButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    replaceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    onButton.setToggleState(true, juce::NotificationType::dontSendNotification);
}

void MusicMagicAudioProcessorEditor::toggleIO(std::string IO)
{
    if (IO == "Input") {
        inputFocusButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        outputFocusButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        MusMagProcessor.setInput(true);
    } else {
        inputFocusButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        outputFocusButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        MusMagProcessor.setInput(false);
    }
}

//changing appearance of input track
void MusicMagicAudioProcessorEditor::updateInputTrackDesign()
{
    if (MusMagProcessor.getNumSamplerSounds() == 1) {
        input_load_box.setButtonText("Track Segment Loaded");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 100, 200));
        input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    } else {
        input_load_box.setButtonText("Drag and Drop Or Click To Select");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    }
}

//changing appearance of out track
void MusicMagicAudioProcessorEditor::updateOutputTrackDesign()
{
    if (MusMagProcessor.getNumOutputSounds() == 1) {
        output_track_box.setButtonText("New Music Track");
        output_track_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkmagenta);
        output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    } else {
        output_track_box.setButtonText("Currently No Output");
        output_track_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    }
}

//=================================================================== Making Request
//checking all input fields are valid
bool MusicMagicAudioProcessorEditor::check_valid_request()
{
    //checking action
    bool action_selected = false;
    if (extendButton.getToggleState() || fillButton.getToggleState() ||
        replaceButton.getToggleState() || generateButton.getToggleState() ) action_selected = true;
    //check prompt
    juce::String userInput = userPrompt.getText();
    std::string promptStr = userInput.toStdString();
    
    //returning true if valid
    if (promptStr != "" && action_selected) return true;
    
    //chaning UI if false
    generate_music_button.setButtonText("Invalid Request!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    startTimer(2500);
    return false;
}

//generating request
void MusicMagicAudioProcessorEditor::generate_request()
{
    if (check_valid_request()) {
        //converting to integer
        juce::Value slider_value = RandomnessSlider.getValueObject();
        double slider_double = slider_value.getValue();
        int slider_int = static_cast<int>(slider_double);
        juce::String randomness = juce::String(slider_int);
        juce::String prompt = userPrompt.getText();;
        juce::String action;
        if (extendButton.getToggleState()) {
            action = "Extend";
        } else if (replaceButton.getToggleState()) {
            action = "Replace";
        } else if (fillButton.getToggleState()) {
            action = "Fill";
        } else if (generateButton.getToggleState()) {
            action = "Generate";
        }
        send_request(prompt, action, randomness);
    }
}

//sending request and updating UI elements
void MusicMagicAudioProcessorEditor::send_request(juce::String prompt, juce::String action, juce::String randomness)
{
    //in final send this request to plugin processor
    generate_music_button.setButtonText("Request Sent!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);
    
    startTimer(4000);
}

//resetting elements after making request
void MusicMagicAudioProcessorEditor::timerCallback()
{
    stopTimer();
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    
}

//=================================================================== Drag & Drop IO
bool MusicMagicAudioProcessorEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
    for (auto file : files) {
        if ( file.contains(".wav") || file.contains(".mp3") || file.contains(".aif") ) {
            return true;
        }
    }
    return false;
};

void MusicMagicAudioProcessorEditor::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (auto file : files) {
        if ( isInterestedInFileDrag(file) ) {
            MusMagProcessor.loadInputFile(file);
        }
    }
    updateInputTrackDesign();
};

void MusicMagicAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (event.eventComponent == &output_track_box) {
        juce::StringArray filesToDrag;
        filesToDrag.add( MusMagProcessor.getPath() );
        juce::DragAndDropContainer::performExternalDragDropOfFiles(filesToDrag, false, nullptr, nullptr);
    }
}

//=================================================================== NEW

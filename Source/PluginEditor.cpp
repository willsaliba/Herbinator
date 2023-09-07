#include "PluginProcessor.h"
#include "PluginEditor.h"

//======================================================================BUILDING PAGE

//CONSTRUCTOR
MusicMagicAudioProcessorEditor::MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor& p)
    : AudioProcessorEditor (&p), MusMagProcessor (p)
{
    //INPUTPUT COMPONENTS
    initialiseInputComponents();
    
    //ACTION BUTTONS
    extendButton.setButtonText("Extend");
    extendButton.onClick = [&]() { toggleOn(extendButton, "Extend"); };
    addAndMakeVisible(extendButton);
    fillButton.setButtonText("Fill");
    fillButton.onClick = [&]() { toggleOn(fillButton, "Fill"); };
    addAndMakeVisible(fillButton);
    replaceButton.setButtonText("Replace");
    replaceButton.onClick = [&]() { toggleOn(replaceButton, "Replace"); };
    addAndMakeVisible(replaceButton);
    generateButton.setButtonText("Generate");
    generateButton.onClick = [&]() { toggleOn(generateButton, "Generate"); };
    addAndMakeVisible(generateButton);
    
    //COVERS
    input_cover.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    input_cover.setAlpha(0.5f);
    extend_cover.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    extend_cover.setAlpha(0.5f);
    
    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30);
    RandomnessSlider.setRange(0, 100);
    RandomnessSlider.setNumDecimalPlacesToDisplay(0);
    RandomnessSlider.setValue(50);
    addAndMakeVisible(RandomnessSlider);
    
    //EXTEND CONTROLS
    extendSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    extendSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    extendSlider.setRange(0, 10);
    extendSlider.setNumDecimalPlacesToDisplay(0);
    extendSlider.setValue(5);
    addAndMakeVisible(extendSlider);
    leftExtendButton.onClick = [&]() { toggleSide(leftExtendButton); };
    addAndMakeVisible(leftExtendButton);
    rightExtendButton.onClick = [&]() { toggleSide(rightExtendButton); };
    addAndMakeVisible(rightExtendButton);
    
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
    g.fillRect(10, 100, 153, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 100, 153, 220), 2.0f);
    
    //action square buttons
    g.setColour(juce::Colour(85, 85, 85));
    g.fillRect(20, 140, 133, 35);
    g.fillRect(20, 185, 133, 35);
    g.fillRect(20, 230, 133, 35);
    g.fillRect(20, 275, 133, 35);
    
    //randomiser square
    g.setColour(juce::Colours::black);
    g.fillRect(173, 100, 153, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(173, 100, 153, 220), 2.0f);
    
    //extend square
    g.setColour(juce::Colours::black);
    g.fillRect(336, 100, 153, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(336, 100, 153, 220), 2.0f);
    
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
    g.drawFittedText("Input\nTrack", 20, 30, 65, 40, juce::Justification::centred, false);
    g.drawFittedText("Action", 35, 95, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Randomiser", 198, 95, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Extend by", 363, 95, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Prompt", 25, 345, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Output\nTrack", 23, 530, 65, 40, juce::Justification::centred, false);
    g.drawFittedText("L", 340, 275, 50, 40, juce::Justification::centred, false);
    g.drawFittedText("R", 430, 275, 50, 40, juce::Justification::centred, false);
    
}

//positioning of active elements
void MusicMagicAudioProcessorEditor::resized()
{
    //input
    input_load_box.setBounds(100, 20, 260, 60);
    input_play_button.setBounds(375, 35, 30, 30);
    input_stop_button.setBounds(410, 35, 30, 30);
    input_delete_button.setBounds(445, 35, 30, 30);
    
    //action buttons
    extendButton.setBounds(25, 148.5, 100, 20);
    fillButton.setBounds(25, 193.5, 100, 20);
    replaceButton.setBounds(25, 238.5, 100, 20);
    generateButton.setBounds(25, 283.5, 100, 20);
    
    //covers
    input_cover.setBounds(10, 10, 480, 80);
    extend_cover.setBounds(336, 100, 153, 220);
    
    //randomness
    RandomnessSlider.setBounds(173, 150, 150, 150);
    
    //extend
    extendSlider.setBounds(345, 135, 130, 130);
    leftExtendButton.setBounds(380, 280, 30, 30);
    rightExtendButton.setBounds(415, 280, 30, 30);
    
    //prompt
    userPrompt.setBounds(105, 355, 360, 30);
    
    //generate music button
    generate_music_button.setBounds(125, 435, 250, 50);
    
    //output
    output_track_box.setBounds(100, 520, 260, 60);
    output_play_button.setBounds(375, 535, 30, 30);
    output_stop_button.setBounds(410, 535, 30, 30);
    output_delete_button.setBounds(445, 535, 30, 30);
    
    //XXX
    selectOutput.setBounds(435, 445, 30, 30);
}

//refactoring to declutter constructor
void MusicMagicAudioProcessorEditor::initialiseInputComponents()
{
    //load input track box
    input_load_box.setButtonText("Drag and Drop Or Click To Select");
    input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_load_box.onClick = [&]() {
        MusMagProcessor.loadInputFile();
        updateInputTrackDesign();
    };
    addAndMakeVisible(input_load_box);
    
    //input play button
    input_play_button.setButtonText("P");
    input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_play_button.onClick = [&]() {
        MusMagProcessor.setInput(true);
        MusMagProcessor.sendMIDInotes();
    };
    addAndMakeVisible(input_play_button);
    
    //input stop button
    input_stop_button.setButtonText("S");
    input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_stop_button.onClick = [&]() {
        MusMagProcessor.setInput(true);
        MusMagProcessor.sendNoSound();
    };
    addAndMakeVisible(input_stop_button);
    
    //delete input button
    input_delete_button.setButtonText("X");
    input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_delete_button.onClick = [&]() {
        MusMagProcessor.sendNoSound();
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
    
    //output play button
    output_play_button.setButtonText("P");
    output_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_play_button.onClick = [&]() {
        MusMagProcessor.setInput(false);
        MusMagProcessor.sendMIDInotes();
    };
    addAndMakeVisible(output_play_button);
    
    //input stop button
    output_stop_button.setButtonText("S");
    output_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_stop_button.onClick = [&]() {
        MusMagProcessor.setInput(false);
        MusMagProcessor.sendNoSound();
    };
    addAndMakeVisible(output_stop_button);
    
    //output delete button
    output_delete_button.setButtonText("X");
    output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_delete_button.onClick = [&]() {
        MusMagProcessor.sendNoSound();
        MusMagProcessor.clearOutputSampler();
        updateOutputTrackDesign();
    };
    addAndMakeVisible(output_delete_button);
}

//========================================================================= GENERAL
//ensuring only 1 action displays on
void MusicMagicAudioProcessorEditor::toggleOn(juce::ToggleButton& onButton, juce::String action)
{
    extendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    fillButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    generateButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    replaceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    onButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    
    //remove grey rect over extend & input
    input_cover.setVisible(false);
    extend_cover.setVisible(false);
    if (action == "Generate") {
        addAndMakeVisible(input_cover);
        addAndMakeVisible(extend_cover);
    } else if (action == "Fill" || action == "Replace") {
        addAndMakeVisible(extend_cover);
    }
}

void MusicMagicAudioProcessorEditor::toggleSide(juce::ToggleButton& onButton)
{
    leftExtendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    rightExtendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    onButton.setToggleState(true, juce::NotificationType::dontSendNotification);
}

//changing appearance of input track
void MusicMagicAudioProcessorEditor::updateInputTrackDesign()
{
    if (MusMagProcessor.getNumSamplerSounds() == 1) {
        input_load_box.setButtonText("Track Segment Loaded");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 100, 200));
        input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
        input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    } else {
        input_load_box.setButtonText("Drag and Drop Or Click To Select");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    }
}

//changing appearance of output track
void MusicMagicAudioProcessorEditor::updateOutputTrackDesign()
{
    if (MusMagProcessor.getNumOutputSounds() == 1) {
        output_track_box.setButtonText("New Music Track");
        output_track_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkmagenta);
        output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        output_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
        output_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    } else {
        output_track_box.setButtonText("Currently No Output");
        output_track_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        output_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        output_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        output_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    }
}

//=================================================================== Making Request

void MusicMagicAudioProcessorEditor::ui_update_invalid_request()
{
    generate_music_button.setButtonText("Invalid Request!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    startTimer(2500);
}

void MusicMagicAudioProcessorEditor::ui_update_request_sent()
{
    generate_music_button.setButtonText("Request Sent!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);
    startTimer(2500);
}

//resetting generate button after request finishes
void MusicMagicAudioProcessorEditor::timerCallback()
{
    stopTimer();
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
}

void MusicMagicAudioProcessorEditor::generate_request()
{
    juce::String prompt = userPrompt.getText();
    juce::String randomness = juce::String(static_cast<int>(RandomnessSlider.getValueObject().getValue()));
    juce::String action;
    if (extendButton.getToggleState())        action = "Extend";
    else if (replaceButton.getToggleState())  action = "Replace";
    else if (fillButton.getToggleState())     action = "Fill";
    else if (generateButton.getToggleState()) action = "Generate";
    else action = "Unselected";
    //send request to processor & update UI
    bool result = MusMagProcessor.process_request(prompt, action, randomness);
    if (result) ui_update_request_sent();
    else ui_update_invalid_request();
}

//=================================================================== Drag & Drop IO
//checking if valid file type
bool MusicMagicAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

//CONSTRUCTOR
MusicMagicAudioProcessorEditor::MusicMagicAudioProcessorEditor (MusicMagicAudioProcessor& p)
    : AudioProcessorEditor (&p), MusMagProcessor (p) {
    
    //input
    input_load_box.setButtonText("Drag and Drop Track Segment");
    input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(input_load_box);
    
    input_play_button.setButtonText("P");
    input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_delete_button.onClick = [&]() { MusMagProcessor.playSample(); };
    addAndMakeVisible(input_play_button);
    
    input_delete_button.setButtonText("D");
    input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_delete_button.onClick = [&]() { MusMagProcessor.clearInputTrack(); updateInputTrackDesign(); };
    addAndMakeVisible(input_delete_button);
    
    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30); //pos, read only, w, h
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
    
    //generate music button
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    generate_music_button.onClick = [&]() { generate_request(); };
    addAndMakeVisible(generate_music_button);

    //output
    output_play_button.setButtonText("P");
    output_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(output_play_button);
    output_copy_button.setButtonText("C");
    output_copy_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(output_copy_button);
        
    //size of window
    setSize (500, 600);
}

//DESTRUCTOR
MusicMagicAudioProcessorEditor::~MusicMagicAudioProcessorEditor() {
}

//non-functional graphics
void MusicMagicAudioProcessorEditor::paint (juce::Graphics& g) {
    
    //background colour
    g.fillAll(juce::Colour(0x21, 0x21, 0x21));
        
    //input square
    g.setColour(juce::Colours::black); //inner
    g.fillRect(10, 10, 480, 80);
    g.setColour(juce::Colour(25, 160, 250)); //border
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
    g.drawFittedText("Input Track", 25, 25, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Action", 110, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Randomiser", 355, 100, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Prompt", 25, 345, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Output Track", 25, 525, 150, 50, juce::Justification::left, false);
}

//positioning of active elements
void MusicMagicAudioProcessorEditor::resized() {
    
    //input
    input_load_box.setBounds(135, 20, 250, 60);
    input_play_button.setBounds(400, 35, 30, 30);
    input_delete_button.setBounds(440, 35, 30, 30);
    
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
    output_play_button.setBounds(400, 535, 30, 30);
    output_copy_button.setBounds(440, 535, 30, 30);
    
    //in final \X/
    randBut.setBounds(160, 520, 90, 20); actionBut.setBounds(270, 520, 100, 20);
    promptBut.setBounds(160, 550, 210, 20);
    
}

//==============================================================================Custom Functions

//checking if is valid audio file
bool MusicMagicAudioProcessorEditor::isInterestedInFileDrag (const juce::StringArray &files) {
    for (auto file : files) {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif")) {
            return true;
        }
    }
    return false;
}

//Calling plugin processor to load in files
void MusicMagicAudioProcessorEditor::filesDropped (const juce::StringArray &files, int x, int y) {
    //loading in file
    for (auto file : files) {
        if ( isInterestedInFileDrag(file) ) MusMagProcessor.loadFile(file);
    }
    updateInputTrackDesign();
}

//changing appearance of input track based on whether empty or not
void MusicMagicAudioProcessorEditor::updateInputTrackDesign() {
    if (MusMagProcessor.getNumSamplerSounds() > 0) {
        input_load_box.setButtonText("Track Segment Loaded");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 100, 200));
    } else {
        input_load_box.setButtonText("Drag and Drop Track Segment");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    }
}

//ensuring only 1 action button can be pressed at a time
void MusicMagicAudioProcessorEditor::toggleOn(juce::ToggleButton& onButton) {
    extendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    fillButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    generateButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    replaceButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    onButton.setToggleState(true, juce::NotificationType::dontSendNotification);
}

//checking if all input fields are valid to make request
bool MusicMagicAudioProcessorEditor::check_valid_request() {
    //checking action
    bool action_selected = false;
    if (extendButton.getToggleState() || fillButton.getToggleState() ||
        replaceButton.getToggleState() || generateButton.getToggleState() ) {
        action_selected = true;
    }
    //check prompt
    juce::String userInput = userPrompt.getText();
    std::string promptStr = userInput.toStdString();
    
    if (promptStr != "" && action_selected) return true;
    
    generate_music_button.setButtonText("Invalid Request!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    startTimer(2500);
    return false;
}

//generating request
void MusicMagicAudioProcessorEditor::generate_request() {
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

//sending the request and updating UI elements
void MusicMagicAudioProcessorEditor::send_request(juce::String prompt, juce::String action, juce::String randomness) {
    //in final send this request to plugin processor
    generate_music_button.setButtonText("Request Sent!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);
    //in final \X/
    promptBut.setButtonText("P: " + prompt); randBut.setButtonText("A: " + action);
    actionBut.setButtonText("R: " + randomness);
    addAndMakeVisible(promptBut); addAndMakeVisible(actionBut); addAndMakeVisible(randBut);
    startTimer(4000);
}

//resetting elements after making request
void MusicMagicAudioProcessorEditor::timerCallback() {
    stopTimer();
    generate_music_button.setButtonText("GENERATE MUSIC!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    //in final \X/
    promptBut.setVisible(false); actionBut.setVisible(false); randBut.setVisible(false);
}




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
    fillButton.setButtonText("In-Fill");
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
    custom_slider_cover.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    custom_slider_cover.setAlpha(0.5f);
    randomiser_cover.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    randomiser_cover.setAlpha(0.5f);
    
    //RANDOMNESS SLIDER
    RandomnessSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RandomnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 30);
    RandomnessSlider.setRange(0, 100);
    RandomnessSlider.setNumDecimalPlacesToDisplay(0);
    RandomnessSlider.setValue(50);
    addAndMakeVisible(RandomnessSlider);
    
    //CUSTOM CONTROLS
    extendSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    extendSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 25);
    extendSlider.setRange(0, 10);
    extendSlider.setNumDecimalPlacesToDisplay(1);
    extendSlider.setValue(5);
    addAndMakeVisible(extendSlider);
    
    juce::Font myFont(20.0f); myFont = myFont.boldened();
    customSliderLabel.setFont(myFont);
    customSliderLabel.setText("Extend by", juce::NotificationType::dontSendNotification);
    customSliderLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(customSliderLabel);
    
    //EXTEND CONTROLS
    leftExtendButton.onClick = [&]() { toggleSide(leftExtendButton, "left"); };
    addAndMakeVisible(leftExtendButton);
    rightExtendButton.onClick = [&]() { toggleSide(rightExtendButton, "right"); };
    addAndMakeVisible(rightExtendButton);
    side = "null";
    
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
    generate_music_button.onClick = [&]() { ui_update_request_sent(); };
    addAndMakeVisible(generate_music_button);
    sendingRequest = false;

    //OUTPUT COMPONENTS
    initialiseOutputComponents();

    //size of window
    setSize (500, 640);
}

//DESTRUCTOR
MusicMagicAudioProcessorEditor::~MusicMagicAudioProcessorEditor()
{
    firstStart.removeListener(this);
    firstEnd.removeListener(this);
}

//non-functional graphics
void MusicMagicAudioProcessorEditor::paint(juce::Graphics& g)
{
    //background colour
    g.fillAll(juce::Colour(0x21, 0x21, 0x21));
        
    //input square
    if (infillMode) {
        g.setColour(juce::Colours::black);
        g.fillRect(10, 10, 480, 120);
        g.setColour(juce::Colour(25, 160, 250));
        g.drawRect(juce::Rectangle<int>(10, 10, 480, 120), 2.0f);
    } else {
        g.setColour(juce::Colours::black);
        g.fillRect(10, 10, 480, 80);
        g.setColour(juce::Colour(25, 160, 250));
        g.drawRect(juce::Rectangle<int>(10, 10, 480, 80), 2.0f);
    }
    
    //action square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 140, 153, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 140, 153, 220), 2.0f);
    
    //action square buttons
    g.setColour(juce::Colour(85, 85, 85));
    g.fillRect(20, 180, 133, 35);
    g.fillRect(20, 225, 133, 35);
    g.fillRect(20, 270, 133, 35);
    g.fillRect(20, 315, 133, 35);
    
    //randomiser square
    g.setColour(juce::Colours::black);
    g.fillRect(173, 140, 153, 220);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(173, 140, 153, 220), 2.0f);
    
    //custom square
    g.setColour(juce::Colours::black);
    g.fillRect(336, 140, 153, 160);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(336, 140, 153, 160), 2.0f);
    
    //extend square
    g.setColour(juce::Colours::black);
    g.fillRect(336, 310, 153, 50);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(336, 310, 153, 50), 2.0f);
    
    //prompt square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 370, 480, 80);
    g.setColour(juce::Colour(250, 240, 0));
    g.drawRect(juce::Rectangle<int>(10, 370, 480, 80), 2.0f);
    
    //output square
    g.setColour(juce::Colours::black);
    g.fillRect(10, 550, 480, 80);
    g.setColour(juce::Colour(190, 25, 250));
    g.drawRect(juce::Rectangle<int>(10, 550, 480, 80), 2.0f);
    
    //text for each box
    juce::Font myFont(20.0f);
    myFont = myFont.boldened();
    g.setFont(myFont);
    g.setColour(juce::Colours::white);
    if (!infillMode) g.drawFittedText("Input\nTrack", 20, 30, 65, 40, juce::Justification::centred, false);
    else g.drawFittedText("Start &\nEnd\nTracks", 20, 50, 65, 40, juce::Justification::centred, false);
    g.drawFittedText("Action", 35, 135, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Randomiser", 198, 135, 100, 50, juce::Justification::centred, false);
    g.drawFittedText("Prompt", 25, 385, 100, 50, juce::Justification::left, false);
    g.drawFittedText("Output\nTrack", 23, 570, 65, 40, juce::Justification::centred, false);
    g.drawFittedText("L", 340, 315, 50, 40, juce::Justification::centred, false);
    g.drawFittedText("R", 430, 315, 50, 40, juce::Justification::centred, false);
}

//positioning of active elements
void MusicMagicAudioProcessorEditor::resized()
{
    //first input track
    input_load_box.setBounds(100, 22.5, 260, 55);
    input_play_button.setBounds(375, 35, 30, 30);
    input_stop_button.setBounds(410, 35, 30, 30);
    input_delete_button.setBounds(445, 35, 30, 30);
    firstStart.setBounds(95, 10, 270, 20);
    firstEnd.setBounds(95, 70, 270, 20);
    //second input track
    sec_input_load_box.setBounds(295, 22.5, 180, 55);
    sec_input_play_button.setBounds(335, 90, 30, 30);
    sec_input_stop_button.setBounds(370, 90, 30, 30);
    sec_input_delete_button.setBounds(405, 90, 30, 30);
    secStart.setBounds(290, 10, 190, 20);
    secEnd.setBounds(290, 70, 190, 20);
    
    //action buttons    
    generateButton.setBounds(25, 188.5, 100, 20);
    replaceButton.setBounds(25, 233.5, 100, 20);
    extendButton.setBounds(25, 278.5, 100, 20);
    fillButton.setBounds(25, 323.5, 100, 20);
    
    //covers
    input_cover.setBounds(10, 10, 480, 80);
    custom_slider_cover.setBounds(336, 140, 153, 160);
    extend_cover.setBounds(336, 310, 153, 50);
    randomiser_cover.setBounds(173, 140, 153, 220);
    
    //randomness
    RandomnessSlider.setBounds(173, 190, 150, 150);
    //custom
    extendSlider.setBounds(345, 175, 130, 110);
    customSliderLabel.setBounds(363, 135, 100, 50);
    //extend
    leftExtendButton.setBounds(380, 320, 30, 30);
    rightExtendButton.setBounds(415, 320, 30, 30);
    //prompt
    userPrompt.setBounds(105, 395, 360, 30);
    //generate music button
    generate_music_button.setBounds(125, 475, 250, 50);
    
    //output
    output_track_box.setBounds(100, 560, 260, 60);
    output_play_button.setBounds(375, 575, 30, 30);
    output_stop_button.setBounds(410, 575, 30, 30);
    output_delete_button.setBounds(445, 575, 30, 30);
}

//refactoring to declutter constructor
void MusicMagicAudioProcessorEditor::initialiseInputComponents()
{
    //load input track box
    input_load_box.setButtonText("Drag and Drop Or Click");
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
        MusMagProcessor.setTrackPlaying("in1");
        MusMagProcessor.sendMIDInotes();
    };
    addAndMakeVisible(input_play_button);
    //input stop button
    input_stop_button.setButtonText("S");
    input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    input_stop_button.onClick = [&]() {
        MusMagProcessor.setTrackPlaying("in1");
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
    
    //first start & cover
    firstStart.setRange(0.0, 100.0, 1.0);
    firstStart.setValue(0.0);
    firstStart.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    firstStart.addListener(this);
    firstStart.setEnabled(false);
    addAndMakeVisible(firstStart);
    firstStartCover.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    firstStartCover.setAlpha(0.5f);
    firstStartCover.setAlwaysOnTop(true);
    //first end & cover
    firstEnd.setRange(0.0, 100.0, 1.0);
    firstEnd.setValue(100.0);
    firstEnd.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    firstEnd.addListener(this);
    firstEnd.setEnabled(false);
    addAndMakeVisible(firstEnd);
    firstEndCover.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    firstEndCover.setAlpha(0.5f);
    firstEndCover.setAlwaysOnTop(true);
    //second start & cover
    secStart.setRange(0.0, 100.0, 1.0);
    secStart.setValue(0.0);
    secStart.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    secStart.addListener(this);
    secStart.setAlwaysOnTop(true);
    secStart.setEnabled(false);
    secStartCover.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    secStartCover.setAlpha(0.5f);
    secStartCover.setAlwaysOnTop(true);
    //second end & cover
    secEnd.setRange(0.0, 100.0, 1.0);
    secEnd.setValue(100.0);
    secEnd.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    secEnd.addListener(this);
    secEnd.setAlwaysOnTop(true);
    secEnd.setEnabled(false);
    secEndCover.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    secEndCover.setAlpha(0.5f);
    secEndCover.setAlwaysOnTop(true);
    
    //second input track box for infill
    infillMode = false;
    sec_input_load_box.setButtonText("Drag and Drop Or Click");
    sec_input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    sec_input_load_box.onClick = [&]() {
        MusMagProcessor.loadSecondFile();
        updateSecInputTrackDesign();
    };
    //sec input play button
    sec_input_play_button.setButtonText("P");
    sec_input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    sec_input_play_button.onClick = [&]() {
        MusMagProcessor.setTrackPlaying("in2");
        MusMagProcessor.sendMIDInotes();
    };
    //sec input stop button
    sec_input_stop_button.setButtonText("S");
    sec_input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    sec_input_stop_button.onClick = [&]() {
        MusMagProcessor.setTrackPlaying("in2");
        MusMagProcessor.sendNoSound();
    };
    //delete input button
    sec_input_delete_button.setButtonText("X");
    sec_input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    sec_input_delete_button.onClick = [&]() {
        MusMagProcessor.sendNoSound();
        MusMagProcessor.clearSecondSampler();
        updateSecInputTrackDesign();
    };
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
        MusMagProcessor.setTrackPlaying("out");
        MusMagProcessor.sendMIDInotes();
    };
    addAndMakeVisible(output_play_button);
    
    //input stop button
    output_stop_button.setButtonText("S");
    output_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    output_stop_button.onClick = [&]() {
        MusMagProcessor.setTrackPlaying("out");
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
    
    //resetting everything
    input_cover.setVisible(false);
    custom_slider_cover.setVisible(false);
    extend_cover.setVisible(false);
    randomiser_cover.setVisible(false);
    
    infillMode = false;
    
    //resizing input componenents
    input_load_box.setBounds(100, 20, 260, 60);
    input_play_button.setBounds(375, 35, 30, 30);
    input_stop_button.setBounds(410, 35, 30, 30);
    input_delete_button.setBounds(445, 35, 30, 30);
    firstStart.setBounds(95, 10, 270, 20);
    firstStart.setValue(0.0);
    firstStartCover.setVisible(false);
    firstEnd.setBounds(95, 70, 270, 20);
    firstEnd.setValue(100.0);
    firstEndCover.setVisible(false);
    
    //making second input componenents invisible/reset
    sec_input_load_box.setVisible(false);
    sec_input_play_button.setVisible(false);
    sec_input_stop_button.setVisible(false);
    sec_input_delete_button.setVisible(false);
    secStart.setVisible(false);
    secEnd.setVisible(false);
    secStartCover.setVisible(false);
    secEndCover.setVisible(false);
    secStart.setValue(0.0);
    secEnd.setValue(100.0);
    
    if (action == "Generate") {
        addAndMakeVisible(input_cover);
        addAndMakeVisible(custom_slider_cover);
        addAndMakeVisible(extend_cover);
    } else if (action == "Fill") {
        infillMode = true;
        customSliderLabel.setText("Gap Length", juce::NotificationType::dontSendNotification);
        addAndMakeVisible(extend_cover);
        addAndMakeVisible(randomiser_cover);
        //first track
        input_load_box.setBounds(100, 22.5, 180, 55);
        input_play_button.setBounds(140, 90, 30, 30);
        input_stop_button.setBounds(175, 90, 30, 30);
        input_delete_button.setBounds(210, 90, 30, 30);
        //first segment percision
        firstStart.setBounds(95, 10, 190, 20);
        firstEnd.setBounds(95, 70, 190, 20);
        //second track
        addAndMakeVisible(secStart);
        addAndMakeVisible(secEnd);
        addAndMakeVisible(sec_input_load_box);
        addAndMakeVisible(sec_input_play_button);
        addAndMakeVisible(sec_input_stop_button);
        addAndMakeVisible(sec_input_delete_button);
        
    } else if (action == "Replace") {
        addAndMakeVisible(extend_cover);
        addAndMakeVisible(custom_slider_cover);
    } else {
        addAndMakeVisible(randomiser_cover);
        customSliderLabel.setText("Extend by", juce::NotificationType::dontSendNotification);
    }
    repaint();
}

void MusicMagicAudioProcessorEditor::toggleSide(juce::ToggleButton& onButton, juce::String sideSelected)
{
    leftExtendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    rightExtendButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    onButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    side = sideSelected;
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
        firstStart.setEnabled(true);
        firstEnd.setEnabled(true);
    } else {
        input_load_box.setButtonText("Drag and Drop Or Click");
        input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        firstStart.setEnabled(false);
        firstEnd.setEnabled(false);
    }
}

void MusicMagicAudioProcessorEditor::updateSecInputTrackDesign()
{
    if (MusMagProcessor.getSecondNumSounds() == 1) {
        sec_input_load_box.setButtonText("Track Segment Loaded");
        sec_input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 100, 200));
        sec_input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        sec_input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
        sec_input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
        secStart.setEnabled(true);
        secEnd.setEnabled(true);
    } else {
        sec_input_load_box.setButtonText("Drag and Drop Or Click");
        sec_input_load_box.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        sec_input_delete_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        sec_input_play_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        sec_input_stop_button.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        secStart.setEnabled(false);
        secEnd.setEnabled(false);
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

void MusicMagicAudioProcessorEditor::ui_red_update(juce::String msg)
{
    generate_music_button.setButtonText(msg);
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    startTimer(3500);
}

void MusicMagicAudioProcessorEditor::ui_update_request_sent()
{
    sendingRequest = true;
    generate_music_button.setButtonText("Request Sent!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);
    startTimer(750);
}

void MusicMagicAudioProcessorEditor::ui_request_success()
{
    generate_music_button.setButtonText("Request Succeeded!");
    generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colour(190, 0, 180));
    startTimer(2500);
}

//calling generate_request // resetting UI appearance
void MusicMagicAudioProcessorEditor::timerCallback()
{
    stopTimer();
    if (sendingRequest) {
        generate_request();
    } else {
        generate_music_button.setButtonText("GENERATE MUSIC!");
        generate_music_button.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    }
}

void MusicMagicAudioProcessorEditor::generate_request()
{
    //getting parameters for request
    juce::String prompt = userPrompt.getText();
    juce::String randomness = juce::String(static_cast<int>(RandomnessSlider.getValueObject().getValue()));
    juce::String time = juce::String(static_cast<float>(extendSlider.getValueObject().getValue()));
    juce::String action;
    if (extendButton.getToggleState())        action = "Extend";
    else if (replaceButton.getToggleState())  action = "Replace";
    else if (fillButton.getToggleState())     action = "Fill";
    else if (generateButton.getToggleState()) action = "Generate";
    else action = "Unselected";
    //send request to processor & update UI
    sendingRequest = false;
    if (MusMagProcessor.process_request(prompt, action, randomness, time, side)) {
        MusMagProcessor.send_request_to_model(prompt, action, randomness, time, side);
        if (MusMagProcessor.getNumOutputSounds() == 0) {
            ui_red_update("Request Valid but Failed. Ensure the Model's Server is Active");
        } else {
            ui_request_success();
            updateOutputTrackDesign();
        }
    }
    else ui_red_update("Request Invalid");
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
    updateSecInputTrackDesign();
};

void MusicMagicAudioProcessorEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (event.eventComponent == &output_track_box) {
        juce::StringArray filesToDrag;
        filesToDrag.add( MusMagProcessor.getPath() );
        juce::DragAndDropContainer::performExternalDragDropOfFiles(filesToDrag, false, nullptr, nullptr);
    }
}


//=================================================================== Precise Segment Selection

//changing slider values and covers
void MusicMagicAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (!infillMode && MusMagProcessor.getNumSamplerSounds() == 1) {
        //when not infill
        if (slider == &firstStart) {
            int value = static_cast<int>(firstStart.getValue());
            value = value * 2.6;
            firstStartCover.setBounds(100, 22.5, value, 55);
        } else if (slider == &firstEnd) {
            int value = 100 - static_cast<int>(firstEnd.getValue());
            value = value * 2.6;
            firstEndCover.setBounds(360-value, 22.5, value, 55);
        }
    } else {
        //infill mode
        if (slider == &firstStart && MusMagProcessor.getNumSamplerSounds() == 1) {
            int value = static_cast<int>(firstStart.getValue());
            value = value * 1.8;
            firstStartCover.setBounds(100, 22.5, value, 55);
        } else if (slider == &firstEnd && MusMagProcessor.getNumSamplerSounds() == 1) {
            int value = 100 - static_cast<int>(firstEnd.getValue());
            value = value * 1.8;
            firstEndCover.setBounds(280-value, 22.5, value, 55);
        } else if (slider == &secStart && MusMagProcessor.getSecondNumSounds() == 1) {
            int value = static_cast<int>(secStart.getValue());
            value = value * 1.8;
            secStartCover.setBounds(295, 22.5, value, 55);
        } else if (slider == &secEnd && MusMagProcessor.getSecondNumSounds() == 1) {
            int value = 100 - static_cast<int>(secEnd.getValue());
            value = value * 1.8;
            secEndCover.setBounds(475-value, 22.5, value, 55);
        }
    }
    addAndMakeVisible(firstStartCover);
    addAndMakeVisible(firstEndCover);
    addAndMakeVisible(secStartCover);
    addAndMakeVisible(secEndCover);
}

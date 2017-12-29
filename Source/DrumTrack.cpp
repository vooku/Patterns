#include "DrumTrack.h"

#define DOT_D 10
#define frac(x) (x - floor(x))

DrumTrack::DrumTrack()
    : DrumTrack("", 36, false, 1.0f, 4, 127, false)
{
}

DrumTrack::DrumTrack(const std::string& name,
                     juce::int8 note,
                     bool mute,
                     float probability,
                     int quantization,
                     int velocity,
                     bool offset)
    : mName(name),
      mNote(note),
      mActive(false),
      mOffBeatHit(false),
      mLastPos(1)
{
    mMuteButton.setButtonText("Mute");
    mMuteButton.setColour(TextButton::buttonOnColourId, COLOR_HIGHLIGHT);
    mMuteButton.setColour(TextButton::textColourOffId, COLOR_HIGHLIGHT);
    mMuteButton.setToggleState(mute, false);
    
    mMuteButton.addListener(this);

    mProbSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mProbSlider.setRange(0.0, 1.0, 0.01);
    mProbSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    mProbSlider.setTextValueSuffix(" Probability");
    mProbSlider.setValue(probability);
    mProbSlider.setColour(Slider::thumbColourId, COLOR_HIGHLIGHT);

    mProbSlider.addListener(this);

    mQuantSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mQuantSlider.setRange(1.0, 32.0, 1.0);
    mQuantSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    mQuantSlider.setTextValueSuffix(" Quantization");
    mQuantSlider.setValue(quantization);
    mQuantSlider.setColour(Slider::thumbColourId, COLOR_HIGHLIGHT);

    mQuantSlider.addListener(this);

    mVelSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mVelSlider.setRange(0.0, 127.0, 1.0);
    mVelSlider.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    mVelSlider.setTextValueSuffix(" Velocity");
    mVelSlider.setValue(velocity);
    mVelSlider.setColour(Slider::thumbColourId, COLOR_HIGHLIGHT);
    mVelSlider.addListener(this);

    mOffBeatButton.setButtonText("Off Beat");
    mOffBeatButton.setColour(TextButton::buttonOnColourId, COLOR_HIGHLIGHT);
    mOffBeatButton.setColour(TextButton::textColourOffId, COLOR_HIGHLIGHT);
    mOffBeatButton.setToggleState(offset, false);

    mOffBeatButton.addListener(this);

    mNoteEditor.setText(std::to_string(note));
    mNoteEditor.setTextToShowWhenEmpty("Note", COLOR_FRAME);
    mNoteEditor.setInputFilter(new TextEditor::LengthAndCharacterRestriction(3, "0123456789"), true);

    mNoteEditor.addListener(this);
}

void DrumTrack::paint(Graphics& g, int x, int y, int w) const
{
    g.setColour(COLOR_HIGHLIGHT);
    g.drawFittedText(mName, x, y, w, 30, Justification::centred, 1);

    g.setColour(COLOR_FRAME);
    float ringD = 1.2f * DOT_D;
    g.drawEllipse(x + 0.5f * (w - ringD), y + 35 - 0.5f * ringD, ringD, ringD, 2.0f);

    if (mActive) {
        g.setColour(COLOR_HIGHLIGHT);
        g.fillEllipse(x + 0.5f * (w - DOT_D), y + 35 - 0.5f * DOT_D, DOT_D, DOT_D);
    }
}

void DrumTrack::resized(int x, int y, int w)
{
    mMuteButton.setBounds(x + int(0.1 * w), y + 50, 0.8 * w, 20);
    mProbSlider.setBounds(x, y + 70, w, w);
    mQuantSlider.setBounds(x, y + 110, w, w);
    mVelSlider.setBounds(x, y + 150, w, w);
    mOffBeatButton.setBounds(x + int(0.1 * w), y + 200, 0.8 * w, 30);
    mNoteEditor.setBounds(x + int(0.1 * w), y + 240, 0.8 * w, 25);
}

void DrumTrack::update()
{
    mMuteButton.setToggleState(*mMuteParam, false);
    mProbSlider.setValue(*mProbParam);
    mQuantSlider.setValue(*mQuantParam);
    mVelSlider.setValue(*mVelParam);
    mOffBeatButton.setToggleState(*mOffBeatParam, false);
}

void DrumTrack::process(MidiBuffer& midiMessages, const AudioPlayHead::CurrentPositionInfo& currentPlayHead, float randomNumber)
{
    if (mMuteButton.getToggleState()) {
        stop(midiMessages);
        return;
    }

    double beatLen = (currentPlayHead.timeSigNumerator * 4.0) / (currentPlayHead.timeSigDenominator * *mQuantParam);
    double beatPos = fmod(fmod(currentPlayHead.ppqPosition, currentPlayHead.timeSigNumerator), beatLen);
    
    auto sendOn = [&]() {
        if (*mProbParam >= randomNumber) {
            midiMessages.addEvent(MidiMessage::noteOn(1, mNote, uint8(*mVelParam)), 0);
            mActive = true;
        }
    };

    auto sendOff = [&]() {
        midiMessages.addEvent(MidiMessage::noteOff(1, mNote), 0);
        mActive = false;
    };
    
    if (beatPos >= 0.5 * beatLen) {
        if (!mActive && mOffBeatButton.getToggleState() && !mOffBeatHit) {
            sendOn();
            mOffBeatHit = true;
        }
        if (mActive && !mOffBeatButton.getToggleState()) {
            sendOff();
        }
    }
    if (beatPos < mLastPos) {
        mOffBeatHit = false;

        if (mActive && mOffBeatButton.getToggleState()) {
            sendOff();
        }
        if (!mActive && !mOffBeatButton.getToggleState()) {
            sendOn();
        }
    }
    
    mLastPos = beatPos;
}

void DrumTrack::stop(MidiBuffer& midiMessages)
{
    if (mActive) {
        midiMessages.addEvent(MidiMessage::noteOff(1, mNote), 0);
    }

    mActive = false;
    mLastPos = 1;
}

void DrumTrack::sliderValueChanged(Slider* slider)
{
    ignoreUnused(slider);

    *mProbParam = (float)mProbSlider.getValue();
    *mQuantParam = (int)mQuantSlider.getValue();
    *mVelParam = (int)mVelSlider.getValue();
}

void DrumTrack::buttonClicked(Button* button)
{
    button->setToggleState(!button->getToggleState(), false);

    *mMuteParam = mMuteButton.getToggleState();
    *mOffBeatParam = mOffBeatButton.getToggleState();
}

void DrumTrack::buttonStateChanged(Button* button)
{
    ignoreUnused(button);
}

void DrumTrack::textEditorEscapeKeyPressed(TextEditor& textEditor)
{
    mNoteEditor.setText(std::to_string(mNote));
}

void DrumTrack::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    Component::unfocusAllComponents(); // calls textEditorFocusLost()
}

void DrumTrack::textEditorFocusLost(TextEditor& textEditor)
{
    if (!textEditor.isEmpty()) {
        int newNote = std::stoi(textEditor.getText().toStdString());

        if (newNote >= 0 && newNote < 128) {
            mNote = newNote;
        }
    }

    mNoteEditor.setText(std::to_string(mNote));
}




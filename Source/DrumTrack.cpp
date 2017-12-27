#include "DrumTrack.h"

#define DOT_D 10

DrumTrack::DrumTrack()
    : DrumTrack("", 36, 1.0f, 4)
{
}

DrumTrack::DrumTrack(const std::string& name, juce::int8 note, float probability, int quantization)
    : mName(name),
      mNote(note),
      mActive(false),
      mNextOn(0),
      mNextOff(0)
{
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

    mOffsetButton.setButtonText("Off Beat");
    mOffsetButton.setColour(ToggleButton::tickColourId, COLOR_HIGHLIGHT);
    mOffsetButton.setColour(ToggleButton::tickDisabledColourId, COLOR_HIGHLIGHT);

    mOffsetButton.addListener(this);
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
    mProbSlider.setBounds(x, y + 40, w, w);
    mQuantSlider.setBounds(x, y + 80, w, w);
    mOffsetButton.setBounds(x, y + 130, w, 30);
}

void DrumTrack::update()
{
    mProbSlider.setValue(*mProbParam);
    mQuantSlider.setValue(*mQuantParam);
}

void DrumTrack::process(MidiBuffer & midiMessages, const juce::int64 & timeInSamples, int sampleOffset, const juce::int64& sampleRate, float randomNumber)
{
    if (mActive && mNextOff <= timeInSamples + sampleOffset) {
        midiMessages.addEvent(MidiMessage::noteOff(1, mNote), sampleOffset);

        mActive = false;
    }
    if (mNextOn <= timeInSamples + sampleOffset) {
        if (*mProbParam >= randomNumber) {
            midiMessages.addEvent(MidiMessage::noteOn(1, mNote, uint8(127)), sampleOffset);
            mNextOff = mNextOn + sampleRate / (*mQuantParam * 2);
            mActive = true;
        }
        
        mNextOn += sampleRate / *mQuantParam;
    }
}

void DrumTrack::stop(MidiBuffer& midiMessages)
{
    if (mActive) {
        midiMessages.addEvent(MidiMessage::noteOff(1, mNote), 0);
    }

    mActive = false;
    mNextOn = 0;
    mNextOff = 0;
}

void DrumTrack::sliderValueChanged(Slider* slider)
{
    ignoreUnused(slider);

    *mProbParam = mProbSlider.getValue();
    *mQuantParam = (int)mQuantSlider.getValue();
}

void DrumTrack::buttonClicked(Button* button)
{
    ignoreUnused(button);
}

void DrumTrack::buttonStateChanged(Button* button)
{
    ignoreUnused(button);
}


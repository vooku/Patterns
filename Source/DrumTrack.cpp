#include "DrumTrack.h"

#define DOT_D 10

DrumTrack::DrumTrack()
    : DrumTrack("", 1.0f, 4, false)
{
}

DrumTrack::DrumTrack(const std::string& name, float probability, int quantization, bool offset)
    : mName(name),
      mActive(true) // TODO change this
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

    // offset button
}

void DrumTrack::paint(Graphics& g, int x, int y, int w) const
{
    g.drawFittedText(mName, x, y, w, 30, Justification::centred, 1);

    g.setColour(COLOR_FRAME);
    float ringD = 1.2f * DOT_D;
    g.drawEllipse(x + 0.5 * (w - ringD), y + 35 - 0.5 * ringD, ringD, ringD, 2.0f);
    if (mActive) {
        g.setColour(COLOR_HIGHLIGHT);
        g.fillEllipse(x + 0.5 * (w - DOT_D), y + 35 - 0.5 * DOT_D, DOT_D, DOT_D);
    }
}

void DrumTrack::resized(int x, int y, int w)
{
    mProbSlider.setBounds(x, y + 40, 50, 50);
    mQuantSlider.setBounds(x, y + 80, 50, 50);
}

void DrumTrack::update()
{
    mProbSlider.setValue(*mProbParam);
    mQuantSlider.setValue(*mQuantParam);
}

void DrumTrack::sliderValueChanged(Slider* slider)
{
    *mProbParam = mProbSlider.getValue();
    *mQuantParam = mQuantSlider.getValue();
}



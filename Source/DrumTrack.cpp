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
    mProbability.setSliderStyle(Slider::RotaryVerticalDrag);
    mProbability.setRange(0.0, 1.0, 0.01);
    mProbability.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    mProbability.setTextValueSuffix(" Probability");
    mProbability.setValue(probability);
    mProbability.setColour(Slider::thumbColourId, COLOR_HIGHLIGHT);

    mProbability.addListener(this);

    mQuantization.setSliderStyle(Slider::RotaryVerticalDrag);
    mQuantization.setRange(1.0, 32.0, 1.0);
    mQuantization.setTextBoxStyle(Slider::NoTextBox, false, 90, 30);
    mQuantization.setTextValueSuffix(" Quantization");
    mQuantization.setValue(quantization);
    mQuantization.setColour(Slider::thumbColourId, COLOR_HIGHLIGHT);

    mQuantization.addListener(this);

}

void DrumTrack::paint(Graphics& g, int x, int y, int w) const
{
    g.drawFittedText(mName, x, y, w, 30, Justification::centred, 1);

    if (mActive) {
        g.fillEllipse(x + 0.5 * (w - DOT_D), y + 35 - 0.5 * DOT_D, DOT_D, DOT_D);
    }
}

void DrumTrack::resized(int x, int y, int w)
{
    mProbability.setBounds(x, y + 40, 50, 50);
    mQuantization.setBounds(x, y + 80, 50, 50);
}

void DrumTrack::sliderValueChanged(Slider * slider)
{
    //*processor.quantization = quantization.getValue();
}



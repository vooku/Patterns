/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define DOT_D 10

//==============================================================================
PatternsAudioProcessorEditor::PatternsAudioProcessorEditor (PatternsAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), drawDot(false)
{
    setSize (200, 200);

    // these define the parameters of our slider object
    quantization.setSliderStyle(Slider::RotaryVerticalDrag);
    quantization.setRange(1.0, 32.0, 1.0);
    quantization.setTextBoxStyle(Slider::NoTextBox, false, 90, 20);
    quantization.setPopupDisplayEnabled(true, true, this);
    quantization.setTextValueSuffix(" Quantization");
    quantization.setValue(4.0);

    // this function adds the slider to the editor
    addAndMakeVisible(&quantization);

    // add the listener to the slider
    quantization.addListener(this);

    startTimer(33);
}

PatternsAudioProcessorEditor::~PatternsAudioProcessorEditor()
{
}

//==============================================================================
void PatternsAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::deeppink);
    g.setFont (15.0f);
    g.drawFittedText("PATTERNS", 0, 0, getWidth(), 30, Justification::centred, 1);
    g.drawFittedText(processor.debugText, 0, 30, getWidth(), 30, Justification::centred, 1);

    if (drawDot)
        g.fillEllipse(0.5 * (getWidth() - DOT_D), 0.5 * (getHeight() - DOT_D), DOT_D, DOT_D);
}

void PatternsAudioProcessorEditor::resized()
{
    quantization.setBounds(0.1 * getWidth(), 40, 50, 70);
}

void PatternsAudioProcessorEditor::timerCallback()
{
    repaint(); // TODO remove this

    quantization.setValue(*processor.quantization);

    if (drawDot != processor.midiOut) {
        drawDot = processor.midiOut;
        repaint();
    }
}

void PatternsAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    *processor.quantization = quantization.getValue();
}

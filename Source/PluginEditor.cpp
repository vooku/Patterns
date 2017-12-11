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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);
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
    if (drawDot)
        g.fillEllipse(0.5 * (getWidth() - DOT_D), 0.5 * (getHeight() - DOT_D), DOT_D, DOT_D);
}

void PatternsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void PatternsAudioProcessorEditor::timerCallback()
{
    if (drawDot != processor.midiOut) {
        drawDot = processor.midiOut;
        repaint();
    }
}

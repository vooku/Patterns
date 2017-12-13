/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TRACK_OFFSET 50

//==============================================================================
PatternsAudioProcessorEditor::PatternsAudioProcessorEditor (PatternsAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (200, 200);

    for (int i = 0; i < processor.mTracks.size(); i++) {
        addAndMakeVisible(&processor.mTracks[i]->mProbability);
        addAndMakeVisible(&processor.mTracks[i]->mQuantization);

        processor.mTracks[i]->mProbability.setPopupDisplayEnabled(true, true, this);
        processor.mTracks[i]->mQuantization.setPopupDisplayEnabled(true, true, this);
    }

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

    g.setColour (COLOR_HIGHLIGHT);
    g.setFont (15.0f);
    g.drawFittedText("PATTERNS", 0, 0, getWidth(), 30, Justification::centred, 1);
    //g.drawFittedText(processor.debugText, 0, 30, getWidth(), 30, Justification::centred, 1);

    for (int i = 0; i < processor.mTracks.size(); i++) {
        processor.mTracks[i]->paint(g, (i + 0.5) * TRACK_OFFSET, 30, TRACK_OFFSET);
    }
}

void PatternsAudioProcessorEditor::resized()
{
    for (int i = 0; i < processor.mTracks.size(); i++) {
        processor.mTracks[i]->resized((i + 0.5) * TRACK_OFFSET, 30, TRACK_OFFSET);
    }
}

void PatternsAudioProcessorEditor::timerCallback()
{
    repaint(); // TODO remove this ???

    //quantization.setValue(*processor.quantization);
}

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
    setSize (200, 300);

    for (int i = 0; i < processor.mTracks.size(); i++) {
        addAndMakeVisible(&processor.mTracks[i]->mMuteButton);
        addAndMakeVisible(&processor.mTracks[i]->mProbSlider);
        addAndMakeVisible(&processor.mTracks[i]->mQuantSlider);
        addAndMakeVisible(&processor.mTracks[i]->mVelSlider);
        addAndMakeVisible(&processor.mTracks[i]->mOffBeatButton);
        addAndMakeVisible(&processor.mTracks[i]->mNoteEditor);

        processor.mTracks[i]->mProbSlider.setPopupDisplayEnabled(true, true, this);
        processor.mTracks[i]->mQuantSlider.setPopupDisplayEnabled(true, true, this);
        processor.mTracks[i]->mVelSlider.setPopupDisplayEnabled(true, true, this);
    }

    startTimer(33);
}

PatternsAudioProcessorEditor::~PatternsAudioProcessorEditor()
{
}

//==============================================================================
void PatternsAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (COLOR_HIGHLIGHT);
    g.setFont (15.0f);
    g.drawFittedText(processor.debugText, 0, 0, getWidth(), 30, Justification::centred, 1);

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
    for (int i = 0; i < processor.mTracks.size(); i++) {
        processor.mTracks[i]->update();
    }

    repaint(); // TODO remove this ???
}

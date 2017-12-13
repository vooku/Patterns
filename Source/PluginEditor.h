/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "DrumTrack.h"
#include <vector>

//==============================================================================
/**
*/
class PatternsAudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer,
                                      private Slider::Listener
{
public:
    PatternsAudioProcessorEditor (PatternsAudioProcessor&);
    ~PatternsAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    void sliderValueChanged(Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PatternsAudioProcessor& processor;

    std::vector<DrumTrack> tracks;

    bool drawDot;
    Slider quantization;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternsAudioProcessorEditor)
};

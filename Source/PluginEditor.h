#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PatternsAudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer
{
public:
    PatternsAudioProcessorEditor (PatternsAudioProcessor&);
    ~PatternsAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PatternsAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternsAudioProcessorEditor)
};

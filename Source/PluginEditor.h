#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PatternsAudioProcessorEditor : public AudioProcessorEditor,
                                     public Timer,
                                     private Button::Listener
{
public:
    PatternsAudioProcessorEditor (PatternsAudioProcessor&);
    ~PatternsAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;

    PatternsAudioProcessor& processor;

    TextButton mThroughButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternsAudioProcessorEditor)
};

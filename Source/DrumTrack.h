#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class DrumTrack : private Slider::Listener
{
public:
    AudioParameterFloat* mProbParam;
    AudioParameterInt* mQuantParam;

    Slider mProbSlider;
    Slider mQuantSlider;
    ToggleButton mOffset;

    DrumTrack();
    DrumTrack(const std::string& name, juce::int8 note, float probability, int quantization, bool offset);

    const std::string& getName() const { return mName; }

    // setNote
    
    void paint(Graphics& g, int x, int y, int w) const;
    void resized(int x, int y, int w);
    void update();
    void process(MidiBuffer& midiMessages, const juce::int64& timeInSamples, int sampleOffset, const juce::int64& sampleRate, float randomNumber);
    void stop(MidiBuffer& midiMessages);

private:
    void sliderValueChanged(Slider* slider) override;

    // TODO icon
    std::string mName;
    bool mActive;
    
    // TODO offset
    // TODO midi message

    juce::int64 mNextOn;
    juce::int64 mNextOff;

    juce::int8 mNote;
};


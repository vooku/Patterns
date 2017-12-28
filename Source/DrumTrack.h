#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class DrumTrack : private Slider::Listener, private Button::Listener
{
public:
    AudioParameterFloat* mProbParam;
    AudioParameterInt* mQuantParam;

    Slider mProbSlider;
    Slider mQuantSlider;
    ToggleButton mOffsetButton;

    DrumTrack();
    DrumTrack(const std::string& name, juce::int8 note, float probability, int quantization, bool offset);

    const std::string& getName() const { return mName; }

    // setNote
    
    void paint(Graphics& g, int x, int y, int w) const;
    void resized(int x, int y, int w);
    void update();
    void prepareToPlay(const double& sampleRate);
    void process(MidiBuffer& midiMessages, const AudioPlayHead::CurrentPositionInfo& currentPlayHead, float randomNumber);
    void stop(MidiBuffer& midiMessages);

private:
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;

    // TODO icon
    std::string mName;
    bool mActive;
    
    // TODO midi message

    double mLastPos;

    juce::int8 mNote;
};


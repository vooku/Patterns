#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class DrumTrack : private Slider::Listener, private Button::Listener, private TextEditor::Listener
{
public:
    AudioParameterFloat* mProbParam;
    AudioParameterInt* mQuantParam;
    AudioParameterInt* mVelParam;
    AudioParameterBool* mMuteParam;
    AudioParameterBool* mOffBeatParam;
    

    Slider mProbSlider;
    Slider mQuantSlider;
    Slider mVelSlider;
    TextButton mOffBeatButton;
    TextButton mMuteButton;
    TextEditor mNoteEditor;
    
    DrumTrack();
    DrumTrack(const std::string& name, juce::int8 note, bool mute, float probability, int quantization, int velocity, bool offset);

    const std::string& getName() const { return mName; }
    
    void paint(Graphics& g, int x, int y, int w) const;
    void resized(int x, int y, int w);
    void update();
    void process(MidiBuffer& midiMessages, const AudioPlayHead::CurrentPositionInfo& currentPlayHead, float randomNumber);
    void stop(MidiBuffer& midiMessages);

private:
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;
    void textEditorTextChanged(TextEditor& textEditor) override;

    // TODO icon
    std::string mName;
    bool mActive;
    bool mOffBeatHit;
    
    // TODO midi message

    double mLastPos;

    juce::int8 mNote;
};


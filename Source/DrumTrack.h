#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class DrumTrack : private Slider::Listener
{
public:
    Slider mProbability;
    Slider mQuantization;

    DrumTrack();
    DrumTrack(const std::string& name, float probability, int quantization, bool offset);
    
    void paint(Graphics& g, int x, int y, int w) const;
    void resized(int x, int y, int w);
    //void process(MidiBuffer& midiMessages);

private:
    void sliderValueChanged(Slider* slider) override;

    // TODO icon
    std::string mName;
    bool mActive;
    
    // TODO offset
    // TODO midi message
};


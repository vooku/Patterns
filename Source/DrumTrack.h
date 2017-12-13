#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class DrumTrack
{
public:
    DrumTrack();
    DrumTrack(const std::string& name);
    
    void paint(Graphics& g, int x, int y, int w) const;
    //void resized();
    //void process(MidiBuffer& midiMessages);

private:
    std::string mName;
    bool mActive;
};


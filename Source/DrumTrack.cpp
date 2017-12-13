#include "DrumTrack.h"

#define DOT_D 10

DrumTrack::DrumTrack()
    : DrumTrack("")
{
}

DrumTrack::DrumTrack(const std::string& name)
    : mName(name),
      mActive(true) // TODO change this
{
}

void DrumTrack::paint(Graphics& g, int x, int y, int w) const
{
    g.drawFittedText(mName, x, y, w, 30, Justification::centred, 1);

    if (mActive) {
        g.fillEllipse(x + 0.5 * (w - DOT_D), y + 40 - 0.5 * DOT_D, DOT_D, DOT_D);
    }
}



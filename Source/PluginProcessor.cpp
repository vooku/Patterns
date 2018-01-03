#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PatternsAudioProcessor::PatternsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
      mEngine(mRandomDevice()),
      mDistribution(0.0f, 1.0f),
      debugText("PATTERNS"),
      mMidiThrough(true)
{
    mTracks.push_back(new DrumTrack{ "Kick", 36, false, 1.0f, 2, 127, false });
    mTracks.push_back(new DrumTrack{ "Snare", 38, false, 1.0f, 2, 127, true });
    mTracks.push_back(new DrumTrack{ "Hi-Hat", 42, false, 0.3f, 8, 63, false });
    mTracks.push_back(new DrumTrack{ "Tom", 45, false, 0.3f, 8, 127, false });
    mTracks.push_back(new DrumTrack{ "Mot", 50, true, 0.3f, 8, 127, false });
    mTracks.push_back(new DrumTrack{ "Tah-Ih", 46, true, 0.2f, 8, 63, true });
    mTracks.push_back(new DrumTrack{ "Erans", 40, false, 0.2f, 16, 100, false });
    mTracks.push_back(new DrumTrack{ "Kcik", 35, true, 0.75f, 4, 63, true });

    for (int i = 0; i < mTracks.size(); i++) {
        auto name = mTracks[i]->getName();
        addParameter(mTracks[i]->mMuteParam = new AudioParameterBool(name + " mute",
                                                                     name + " Mute",
                                                                     mTracks[i]->mMuteButton.getToggleState()));
        addParameter(mTracks[i]->mProbParam = new AudioParameterFloat(name + " probability",
                                                                      name + " Probability",
                                                                      mTracks[i]->mProbSlider.getMinimum(),
                                                                      mTracks[i]->mProbSlider.getMaximum(),
                                                                      mTracks[i]->mProbSlider.getValue()));
        addParameter(mTracks[i]->mQuantParam = new AudioParameterInt(name + " quantization",
                                                                     name + " Quantization",
                                                                     (int)mTracks[i]->mQuantSlider.getMinimum(),
                                                                     (int)mTracks[i]->mQuantSlider.getMaximum(),
                                                                     (int)mTracks[i]->mQuantSlider.getValue()));
        addParameter(mTracks[i]->mVelParam = new AudioParameterInt(name + " velocity",
                                                                   name + " Velocity",
                                                                   (int)mTracks[i]->mVelSlider.getMinimum(),
                                                                   (int)mTracks[i]->mVelSlider.getMaximum(),
                                                                   (int)mTracks[i]->mVelSlider.getValue()));
        addParameter(mTracks[i]->mOffBeatParam = new AudioParameterBool(name + " Off Beat",
                                                                       name + " Off Beat",
                                                                       mTracks[i]->mOffBeatButton.getToggleState()));
        
    }
    
}

PatternsAudioProcessor::~PatternsAudioProcessor()
{
    for (auto track : mTracks) {
        delete track;
    }
}

//==============================================================================
const String PatternsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PatternsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PatternsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PatternsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PatternsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PatternsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PatternsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PatternsAudioProcessor::setCurrentProgram (int index)
{
    ignoreUnused(index);
}

const String PatternsAudioProcessor::getProgramName (int index)
{
    ignoreUnused(index);
    return {};
}

void PatternsAudioProcessor::changeProgramName (int index, const String& newName)
{
    ignoreUnused(index, newName);
}

//==============================================================================
void PatternsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(sampleRate, samplesPerBlock);
}

void PatternsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PatternsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PatternsAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{ 
    buffer.clear();
    if (!mMidiThrough)
        midiMessages.clear();
    
    if (AudioPlayHead* ph = getPlayHead()) {
        AudioPlayHead::CurrentPositionInfo currentPlayHead;
        
        if (ph->getCurrentPosition(currentPlayHead)) {
            //debugText = std::to_string(currentPlayHead.ppqPosition - lastppq); lastppq = currentPlayHead.ppqPosition;
            for (auto& track : mTracks) {
                if (currentPlayHead.isPlaying) {
                    track->process(midiMessages, currentPlayHead, mDistribution(mEngine));
                }
                else {
                    track->stop(midiMessages);
                }
            }
        }
    }

}

//==============================================================================
bool PatternsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PatternsAudioProcessor::createEditor()
{
    return new PatternsAudioProcessorEditor (*this);
}

//==============================================================================
void PatternsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto& stream = MemoryOutputStream(destData, true);
    for (const auto& track : mTracks) {
        stream.writeBool(*track->mMuteParam);
        stream.writeFloat(*track->mProbParam);
        stream.writeInt(*track->mQuantParam);
        stream.writeInt(*track->mVelParam);
        stream.writeBool(*track->mOffBeatParam);
        stream.writeString(track->mNoteEditor.getText());
    }    
}

void PatternsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto& stream = MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false);
    for (auto& track : mTracks) {
        if (stream.getNumBytesRemaining() < 2 * sizeof(bool) + sizeof(float) + 2 * sizeof(int) + 1)
            break;
        *track->mMuteParam = stream.readBool();
        *track->mProbParam = stream.readFloat();
        *track->mQuantParam = stream.readInt();
        *track->mVelParam = stream.readInt();
        *track->mOffBeatParam = stream.readBool();
        track->mNoteEditor.setText(stream.readString());
        track->update(true);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PatternsAudioProcessor();
}

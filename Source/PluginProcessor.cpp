/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

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
      debugText("PATTERNS"), lastppq(0)
{
    mTracks.push_back(new DrumTrack{ "Kick", 36, false, 1.0f, 2, 127, false });
    mTracks.push_back(new DrumTrack{ "Snare", 38, false, 1.0f, 4, 127, true });
    mTracks.push_back(new DrumTrack{ "Hi-Hat", 42, true, 0.3f, 8, 127, false });

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
                                                                     mTracks[i]->mQuantSlider.getMinimum(),
                                                                     mTracks[i]->mQuantSlider.getMaximum(),
                                                                     mTracks[i]->mQuantSlider.getValue()));
        addParameter(mTracks[i]->mVelParam = new AudioParameterInt(name + " velocity",
                                                                   name + " Velocity",
                                                                   mTracks[i]->mVelSlider.getMinimum(),
                                                                   mTracks[i]->mVelSlider.getMaximum(),
                                                                   mTracks[i]->mVelSlider.getValue()));
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
    
    if (AudioPlayHead* ph = getPlayHead()) {
        AudioPlayHead::CurrentPositionInfo currentPlayHead;
        
        if (ph->getCurrentPosition(currentPlayHead)) {
            debugText = std::to_string(currentPlayHead.ppqPosition - lastppq); lastppq = currentPlayHead.ppqPosition;
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
    ignoreUnused(destData);
    //MemoryOutputStream(destData, true).writeInt(*quantization);
}

void PatternsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ignoreUnused(data, sizeInBytes);
    //*quantization = MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readInt();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PatternsAudioProcessor();
}

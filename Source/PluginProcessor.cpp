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
      midiOut(false),
      nextBeat(0),
      noteOff(0)
#endif
{
    mTracks.push_back(new DrumTrack{ "Kick", 1.0f, 2, false });
    mTracks.push_back(new DrumTrack{ "Snare", 0.7f, 4, true });
    mTracks.push_back(new DrumTrack{ "Hi-Hat", 0.3f, 16, false });

    addParameter(quantization = new AudioParameterInt("quantization", "Quantization", 1, 32, 4));
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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
    midiMessages.clear();

    if (AudioPlayHead* ph = getPlayHead()) {
        AudioPlayHead::CurrentPositionInfo currentPlayHead;
        
        if (ph->getCurrentPosition(currentPlayHead)) {
            if (currentPlayHead.isPlaying) {
                for (int i = 0; i <= buffer.getNumSamples(); i++) {
                    debugText = " " + std::to_string(currentPlayHead.timeInSamples + i);

                    if (noteOff && noteOff <= currentPlayHead.timeInSamples + i) {
                        midiMessages.addEvent(MidiMessage::noteOff(1, 36), i);

                        midiOut = false;
                        noteOff = 0;
                    }
                    if (nextBeat <= currentPlayHead.timeInSamples + i) {
                        midiMessages.addEvent(MidiMessage::noteOn(1, 36, uint8(127)), i);
                        
                        midiOut = true;
                        noteOff = nextBeat + getSampleRate() / 4;
                        nextBeat += getSampleRate() / 2;
                    }
                }
            }
            else {
                midiMessages.addEvent(MidiMessage::noteOff(1, 36), 0);

                midiOut = false;
                noteOff = 0;
                nextBeat = 0;
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
    MemoryOutputStream(destData, true).writeInt(*quantization);
}

void PatternsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    *quantization = MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readInt();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PatternsAudioProcessor();
}

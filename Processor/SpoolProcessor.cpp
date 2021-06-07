/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#if JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#include "../Standalone/StandaloneApp.cpp"
#endif

#include "SpoolProcessor.h"
#include "../Editor/SpoolEditor.h"
#include "Modules/Sequencer.h"

//==============================================================================
SpoolProcessor::SpoolProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true).withOutput ("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
    sequencer.reset(new Sequencer(this));
    tracks.reset(new Tracks(this));
    commands.setOwner(this);
    commandQueue.FNCommandID = Config::Command::Function;
    startTimer(Config::updateCycle);
}

SpoolProcessor::~SpoolProcessor()
{
    sequencer.reset();
    tracks.reset();
}

void SpoolProcessor::timerCallback() {
    auto time = getCurrentTime();
    
    sequencer->update();
    commandQueue.setCurrentTime(time);
    commandQueue.process(false, false);
    editorTimerCallback(false, false);
    
    commandQueue.setFunctionDown(isFunctionDown);
    commandQueue.setMuteDown(isMuteDown);
    commandQueue.setPlayDown(isPlayDown);
    commandQueue.setRecordDown(isRecordDown);
}

void SpoolProcessor::beatCallback(bool isUpBeat) {
    commandQueue.process(true, isUpBeat);
    tracks->beatCallback(isUpBeat);
}


//==============================================================================
const juce::String SpoolProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpoolProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpoolProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpoolProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpoolProcessor::getTailLengthSeconds() const
{
    return 0.0;
}


// NB: some hosts don't cope very well if you tell them there are 0 programs,
// so this should be at least 1, even if you're not really implementing programs.
int SpoolProcessor::getNumPrograms() {
    return 1;
}

int SpoolProcessor::getCurrentProgram() {
    return 0;
}

void SpoolProcessor::setCurrentProgram (int index) {
}

const juce::String SpoolProcessor::getProgramName (int index) {
    return {};
}

void SpoolProcessor::changeProgramName (int index, const juce::String& newName) {
}

//==============================================================================
void SpoolProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    sequencer->prepareToPlay(samplesPerBlock, sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SpoolProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpoolProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void SpoolProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    sequencer->getNextAudioBlock(buffer);
    tracks->processBlockBefore(buffer, midiMessages);
    buffer.clear();
    tracks->processBlockAfter(buffer, midiMessages);

    
//    auto totalNumInputChannels  = getTotalNumInputChannels();
//    auto totalNumOutputChannels = getTotalNumOutputChannels();
//
//    // In case we have more outputs than inputs, this code clears any output
//    // channels that didn't contain input data, (because these aren't
//    // guaranteed to be empty - they may contain garbage).
//    // This is here to avoid people getting screaming feedback
//    // when they first compile a plugin, but obviously you don't need to keep
//    // this code if your algorithm always overwrites all the output channels.
//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());
//
//    // This is the place where you'd normally do the guts of your plugin's
//    // audio processing...
//    // Make sure to reset the state if your inner loop is processing
//    // the samples and the outer loop is handling the channels.
//    // Alternatively, you can process the samples with the channels
//    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
}

//==============================================================================
bool SpoolProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SpoolProcessor::createEditor() {
    return new SpoolEditor (*this);
}

//==============================================================================
void SpoolProcessor::getStateInformation (juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SpoolProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpoolProcessor();
}

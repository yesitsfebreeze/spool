#if JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#include "Standalone/StandaloneApp.cpp"
#endif

#include "SpoolProcessor.h"
#include "Processor/Modules/Sequencer/Sequencer.h"
#include "GUI/GUI.h"

//==============================================================================
SpoolProcessor::SpoolProcessor():
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true).withOutput ("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
    controlGroups[ControlGroup::Group::A] = new ControlGroup(this);
    controlGroups[ControlGroup::Group::B] = new ControlGroup(this);

    sequencer.reset(new Sequencer(this));
    tracks.reset(new Tracks(this));
    commands.setProcessor(this);
    startTimerHz(Config::UpdateHz);
}

SpoolProcessor::~SpoolProcessor() {
    sequencer.reset();
    tracks.reset();
}

void SpoolProcessor::timerCallback() {
    getCurrentTime();
    sequencer->update();
    commandQueue.process(currentTime, false, false);
    guiTimerCallback(false, false);
}

void SpoolProcessor::beatCallback(int beat,bool isUpBeat) {
    commandQueue.process(currentTime, true, isUpBeat);
    tracks->beatCallback(beat, isUpBeat);
}


//==============================================================================
const juce::String SpoolProcessor::getName() const {
    return JucePlugin_Name;
}

bool SpoolProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpoolProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpoolProcessor::isMidiEffect() const {
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpoolProcessor::getTailLengthSeconds() const {
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
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlock;
    processSpec.numChannels = getTotalNumOutputChannels();
    
    sequencer->prepareToPlay(sampleRate, samplesPerBlock);
    tracks->prepareToPlay(sampleRate, samplesPerBlock);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}


void SpoolProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    sequencer->processBlockBefore(buffer, midiMessages);
    tracks->processBlockBefore(buffer, midiMessages);
    buffer.clear();
    sequencer->processBlockAfter(buffer, midiMessages);
    tracks->processBlockAfter(buffer, midiMessages);
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


//==============================================================================
bool SpoolProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* SpoolProcessor::createEditor() {
    return new GUI (*this);
}

//==============================================================================
void SpoolProcessor::getStateInformation (juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SpoolProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new SpoolProcessor();
}

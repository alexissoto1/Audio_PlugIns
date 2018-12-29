/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultipleFilterAudioProcessor::MultipleFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr)
#endif
{
    
    NormalisableRange<float> cutoffRange (20.0f, 20000.0f);
    NormalisableRange<float> resRange (1.0f, 5.0f);
    NormalisableRange<float> filterMenuRange (0, 2);
    NormalisableRange<float> driveRange (0.f, 1.f);
    NormalisableRange<float> rangeRange (1.f, 100.f);
    
    tree.createAndAddParameter("cutoff", "Cutoff", "Cutoff", cutoffRange, 600.0f, nullptr, nullptr);
    tree.createAndAddParameter("resonance", "Resonance", "Resonance", resRange, 1.f, nullptr, nullptr);
    tree.createAndAddParameter("filterMenu", "FilterMenu", "FilterMenu", filterMenuRange, 0, nullptr, nullptr);
    
    tree.createAndAddParameter("drive", "Drive", "Drive", driveRange, 0.1, nullptr, nullptr);
    tree.createAndAddParameter("range", "Range", "Range", rangeRange, 1.f, nullptr, nullptr);

    
}

MultipleFilterAudioProcessor::~MultipleFilterAudioProcessor()
{
}

//==============================================================================
const String MultipleFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultipleFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultipleFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultipleFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultipleFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultipleFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultipleFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultipleFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String MultipleFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultipleFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MultipleFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate; //pass around sample globaly trough the cpp file
    
    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    stateVariableFilter.reset(); //gets rid of all the garbage values.
    updateFilter();
    stateVariableFilter.prepare(spec);
    
}

void MultipleFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MultipleFilterAudioProcessor::updateFilter(){
    
    int menuChoice = *tree.getRawParameterValue("filterMenu"); //star to difference a pointer. Choosing the type of value that is coming by.
    int cutoff = *tree.getRawParameterValue("cutoff");
    int res = *tree.getRawParameterValue("resonance");
    
    if (menuChoice == 0){
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    } if (menuChoice == 1){
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    }if (menuChoice == 2){
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultipleFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MultipleFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float drive = *tree.getRawParameterValue("drive");
    float range = *tree.getRawParameterValue("range");

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    dsp::AudioBlock<float> block (buffer); //block is equal to buffer trough this statement.
    
    updateFilter();
    
    stateVariableFilter.process(dsp::ProcessContextReplacing<float> (block)); //because we are putting our audio in this block.
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel); //channelData is an array. for every sample, distort the sound.
        
        for (int sample = 0; sample < buffer.getNumSamples(); sample++){

            *channelData *= drive * range; //the more range, the more drive you get.
            *channelData = ((2.f/float_Pi)*atan(*channelData)); //math formula for distortion
        
            channelData++;
            
        }
    }
    
}
//==============================================================================
bool MultipleFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MultipleFilterAudioProcessor::createEditor()
{
    return new MultipleFilterAudioProcessorEditor (*this);
}

//==============================================================================
void MultipleFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultipleFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultipleFilterAudioProcessor();
}

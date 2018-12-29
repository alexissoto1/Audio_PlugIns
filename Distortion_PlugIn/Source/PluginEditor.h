/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MultipleFilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MultipleFilterAudioProcessorEditor (MultipleFilterAudioProcessor&);
    ~MultipleFilterAudioProcessorEditor();
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResvalue;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterMenuChoice;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    Slider filterCutoffDial;
    Slider filterResDial;
    Slider driveSlider; //Creating pointers to sliders
    Slider rangeSlider;
    ComboBox filterMenu;

    
    MultipleFilterAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultipleFilterAudioProcessorEditor)
};

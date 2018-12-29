/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultipleFilterAudioProcessorEditor::MultipleFilterAudioProcessorEditor (MultipleFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (900, 200);
    
    filterMenu.setJustificationType(Justification::centredLeft);
    filterMenu.addItem("Low Pass", 1);
    filterMenu.addItem("Band Pass", 2);
    filterMenu.addItem("High Pass", 3);
    addAndMakeVisible(&filterMenu); //&for the refference
    
    filterCutoffDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);
    
    filterResDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterResDial.setRange(1.0f, 5.0f);
    filterResDial.setValue(2.0f);
    filterResDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterResDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResDial);
    
    driveSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    driveSlider.setRange(0.f, 1.f);
    driveSlider.setValue(0.1f);
    driveSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    driveSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&driveSlider);
    
    rangeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    rangeSlider.setRange(0.f, 500.f);
    rangeSlider.setValue(10.f);
    rangeSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    rangeSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&rangeSlider);
    
    filterCutoffValue = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "cutoff", filterCutoffDial);
    filterResvalue = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "resonance", filterResDial);
    filterMenuChoice = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "filterMenu", filterMenu);
    
    driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "drive", driveSlider);
    rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "range", rangeSlider);
    
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f); //Logarythmic range. Mid point is 1Khz

}

MultipleFilterAudioProcessorEditor::~MultipleFilterAudioProcessorEditor()
{
}

//==============================================================================
void MultipleFilterAudioProcessorEditor::paint (Graphics& g)
{

    Rectangle<int>  titleArea (0, 10, 50, 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Filter", titleArea, Justification::centredTop);
    
    g.drawText("Cutoff", (getWidth() / 5)*1-(100/2), (getHeight()/2)+10, 100, 100, Justification::centred, false);
    g.drawText("Resonance", (getWidth() / 5)*2-(100/2), (getHeight()/2)+10, 100, 100, Justification::centred, false);
    
    g.drawText("Drive", (getWidth() / 5)*3-(100/2), (getHeight()/2)+10, 100, 100, Justification::centred, false);
    g.drawText("Range", (getWidth() / 5)*4-(100/2), (getHeight()/2)+10, 100, 100, Justification::centred, false);
}

void MultipleFilterAudioProcessorEditor::resized()
{

    Rectangle<int> area = getLocalBounds().reduced(40);
    
    filterMenu.setBounds(area.removeFromTop(20));
    
    filterCutoffDial.setBounds((getWidth() / 5)*1-(100/2), (getHeight()/2)-(100/2) + 5, 100, 100);
    filterResDial.setBounds((getWidth() / 5)*2-(100/2), (getHeight()/2)-(100/2) + 5, 100, 100);
    
    driveSlider.setBounds((getWidth() / 5)*3-(100/2), (getHeight()/2)-(100/2) + 5, 100, 100);
    rangeSlider.setBounds((getWidth() / 5)*4-(100/2), (getHeight()/2)-(100/2) + 5, 100, 100);
    
}

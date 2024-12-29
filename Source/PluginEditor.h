/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"



//==============================================================================

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override
    {
        rotaryStartAngle = -juce::MathConstants<float>::pi * 1.5f; // 12 o'clock position
        rotaryEndAngle = juce::MathConstants<float>::pi * 0.5f;
        
        // Calculate the center and radius
        auto radius = juce::jmin(width, height) / 2.0f - 4.0f; // Adjust for padding
        auto centerX = x + width * 0.5f;
        auto centerY = y + height * 0.5f;
        auto rx = centerX - radius;
        auto ry = centerY - radius;

        // Background circle (knob base)
        g.setColour(juce::Colours::skyblue);
        g.fillEllipse(rx, ry, radius * 2.0f, radius * 2.0f);

        // Outline of the knob
        g.setColour(juce::Colours::darkslategrey);
        g.drawEllipse(rx, ry, radius * 2.0f, radius * 2.0f, 0.5f);

        // Map the slider value to the angle
        auto range = slider.getRange();
        float sliderValue = slider.getValue();
        float angle = juce::jmap((float)sliderValue, (float)range.getStart(), (float)range.getEnd(), (float)rotaryStartAngle, (float)rotaryEndAngle);

        // Knob indicator (line)
        auto lineLength = radius * 0.8f;
        auto indicatorX = centerX + std::cos(angle) * lineLength;
        auto indicatorY = centerY + std::sin(angle) * lineLength;

        g.setColour(juce::Colours::snow); // Indicator color
        g.drawLine(centerX, centerY, indicatorX, indicatorY, 2.0f); // Thickness of 2.0f

        // Optional: Draw the value as text
        g.setColour(juce::Colours::white);
        g.setFont(12.0f);
        auto valueText = juce::String(slider.getValue(), 1); // Show 1 decimal place
        g.drawText(valueText + " dB", slider.getLocalBounds(), juce::Justification::centredBottom, false);
    }

};

//==============================================================================
/**
*/
class Boost1AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    private juce::Slider::Listener,
                                    public juce::Timer
{
public:
    Boost1AudioProcessorEditor (Boost1AudioProcessor&);
    ~Boost1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    void sliderValueChanged (juce::Slider *slider) override;
    void timerCallback() override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Boost1AudioProcessor& audioProcessor;
    
    juce::Slider gainSlider;
    juce::Rectangle<int> sliderArea;
    juce::Rectangle<int> volumeArea;
    juce::Rectangle<int> labelArea;
    
    float gainPos;
    
    CustomLookAndFeel customLookAndFeel;
    
    juce::Rectangle<float> barArea;
    
    juce::Rectangle<float> leftBar;
    juce::Rectangle<float> rightBar;
    
    float volume = audioProcessor.volumeLevel;


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Boost1AudioProcessorEditor)
};

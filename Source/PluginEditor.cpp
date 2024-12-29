/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Boost1AudioProcessorEditor::Boost1AudioProcessorEditor (Boost1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    timerCallback();
    setSize (400, 600);
    
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setRange(-30, 30);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled(false, false, this);
    gainSlider.setValue(0.0);
    
    
    gainSlider.addListener(this);
    
    
    gainSlider.setLookAndFeel(&customLookAndFeel);
    
}

Boost1AudioProcessorEditor::~Boost1AudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
}

//==============================================================================

void Boost1AudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.gainVal = gainSlider.getValue();
    if (slider == &gainSlider)
        {
  
            repaint(); // Schedules a redraw of the entire editor
        }
}

void Boost1AudioProcessorEditor::timerCallback()
{
    startTimerHz(30);
    float uVol = audioProcessor.getVolume();
    volume = uVol;
    repaint();
}

//==============================================================================


void Boost1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);
    
    g.setColour (juce::Colours::whitesmoke);

    g.fillRect(volumeArea);
    
    g.setColour (juce::Colours::darkslategrey);
    g.drawRect(labelArea);
    
        // Draw labels
    //g.setColour (juce::Colours::darkslategrey);
    juce::String text = "-24 dB    |    24 dB";
    juce::String text2 = "Gain";
    g.drawFittedText(text, sliderArea.getCentreX() - 58, sliderArea.getCentreY() + 110, g.getCurrentFont().getStringWidth(text), g.getCurrentFont().getHeight(), juce::Justification::centred, 1);
    g.drawFittedText(text2, sliderArea.getCentreX() - 58, sliderArea.getCentreY() - 130, g.getCurrentFont().getStringWidth(text), g.getCurrentFont().getHeight(), juce::Justification::centred, 1);
    
    
    
    
    
    auto rowHeight = labelArea.getHeight()/5;
    auto rowArea = labelArea;
    g.setFont(16.f);
    g.drawText ("24 dB", rowArea.removeFromTop(rowHeight), juce::Justification::centred);
    g.drawLine( labelArea.getRight() - 3, rowArea.getHeight() - rowHeight / 2, labelArea.getRight() + 2, rowArea.getHeight() - rowHeight / 2 );
    g.drawText ("12 dB", rowArea.removeFromTop(rowHeight), juce::Justification::centred);
    g.drawLine( labelArea.getRight() - 3, rowArea.getHeight() - rowHeight / 2, labelArea.getRight() + 2, rowArea.getHeight() - rowHeight / 2 );
    g.drawText ("0 dB", rowArea.removeFromTop(rowHeight), juce::Justification::centred);
    g.drawLine( labelArea.getRight() - 3, rowArea.getHeight() - rowHeight / 2, labelArea.getRight() + 2, rowArea.getHeight() - rowHeight / 2 );
    g.drawText ("-12 dB", rowArea.removeFromTop(rowHeight), juce::Justification::centred);
    g.drawLine( labelArea.getRight() - 3, rowArea.getHeight() - rowHeight / 2, labelArea.getRight() + 2, rowArea.getHeight() - rowHeight / 2 );
    g.drawLine( labelArea.getRight() - 3, (rowArea.getHeight() - rowHeight / 2) * 9, labelArea.getRight() + 2, (rowArea.getHeight() - rowHeight / 2) * 9 );
    g.drawText ("-24 dB", rowArea.removeFromTop(rowHeight), juce::Justification::centred);
     
    
    //g.setColour(juce::Colours::skyblue);
    //g.drawRect(volumeArea);
    
    float normalizedValue = (gainSlider.getValue() - gainSlider.getMinimum()) /
                                (gainSlider.getMaximum() - gainSlider.getMinimum());
    float lineY = volumeArea.getBottom() - (volumeArea.getHeight() * normalizedValue);

    // Calculate the line's width to be 1/3 of volumeArea's width
    int lineWidth = volumeArea.getWidth() / 3;

        // Center the line horizontally in volumeArea
    int lineX = volumeArea.getX() + (volumeArea.getWidth() - lineWidth);



    // Calculate the height of the left volume bar
    float leftBarHeight = leftBar.getHeight() * audioProcessor.getVolume();
    juce::ColourGradient gradientL(
                                   juce::Colours::lightskyblue,
                                   leftBar.getX(),                                 // Start point X
                                   leftBar.getBottom() - leftBarHeight,
                                   juce::Colours::darkslateblue,
                                   leftBar.getX(),                                 // End point X
                                   leftBar.getBottom(),
                                   false
                                  );
    g.setGradientFill(gradientL);
    g.fillRect(leftBar.getX(), leftBar.getBottom() - leftBarHeight, leftBar.getWidth(), leftBarHeight);

    
    float rightBarHeight = rightBar.getHeight() * audioProcessor.getVolume();
    juce::ColourGradient gradientR(
                                   juce::Colours::violet,
                                   rightBar.getX(),                                 // Start point X
                                   rightBar.getBottom() - rightBarHeight,
                                   juce::Colours::indigo,
                                   rightBar.getX(),                                 // End point X
                                   rightBar.getBottom(),
                                   false
                                  );
    
    g.setGradientFill(gradientR);
    g.fillRect(rightBar.getX(), rightBar.getBottom() - rightBarHeight, rightBar.getWidth(), rightBarHeight);
    
    
    
    g.setColour(juce::Colours::red);
    g.drawLine(leftBar.getX(), lineY, lineX + lineWidth, lineY, 1.2f);

    
    

}

void Boost1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    
    sliderArea = bounds.removeFromLeft(bounds.getWidth() / 2);
    sliderArea = sliderArea.withTrimmedTop(bounds.getHeight() / 5)
                           .withTrimmedBottom(bounds.getHeight() / 5);
    gainSlider.setBounds(sliderArea);

    // The remaining half of bounds is now volumeArea
    labelArea = bounds.removeFromLeft(bounds.getWidth() / 3);
    volumeArea = bounds; // bounds has been reduced by removeFromLeft
    
    
    barArea = volumeArea.toFloat();
    
    
    leftBar = barArea.removeFromLeft(barArea.getWidth()/2);
    rightBar = barArea.removeFromRight(barArea.getWidth());
    
}

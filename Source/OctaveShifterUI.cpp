/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "OctaveShifterUI.h"


//=============================================================================
OctaveShifterUI::OctaveShifterUI(OneLittleSynthesizerAudioProcessor& p, AudioProcessorValueTreeState & params)
    : processor(p)
    , parameters(params)
    , currentOctaveShift(0)
    , plusButton("+")
    , minusButton("-")
{
    parameters.addParameterListener("octaveShift", this);
    plusButton.addListener(this);
    minusButton.addListener(this);

    addAndMakeVisible(plusButton);
    addAndMakeVisible(minusButton);

    plusButtonInitColour = plusButton.findColour(TextButton::buttonColourId);
    minusButtonInitColour = minusButton.findColour(TextButton::buttonColourId);

    currentOctaveShift = *parameters.getRawParameterValue("octaveShift");
    refreshButtonColours();
}

//=============================================================================
OctaveShifterUI::~OctaveShifterUI()
{
    parameters.removeParameterListener("octaveShift", this);
    plusButton.removeListener(this);
    minusButton.removeListener(this);
}

//=============================================================================
void OctaveShifterUI::paint(Graphics &)
{
}

//=============================================================================
void OctaveShifterUI::resized()
{
    plusButton.setBounds(0, 0, 20, 20);
    minusButton.setBounds(25, 0, 20, 20);
}

//=============================================================================
void OctaveShifterUI::parameterChanged(const String& parameterID, float newValue )
{
    if(parameterID == "octaveShift")
    {
        currentOctaveShift = newValue;
        refreshButtonColours();
    }
}

//=============================================================================
void OctaveShifterUI::buttonClicked(Button * button)
{
    if(button == &plusButton && currentOctaveShift < OCTAVE_SHIFT_MAX)
    {
        processor.setParameterValue("octaveShift", currentOctaveShift + 1);
    }
    else if( button == &minusButton && currentOctaveShift > OCTAVE_SHIFT_MIN)
    {
        processor.setParameterValue("octaveShift", currentOctaveShift - 1);
    }
}

//=============================================================================
void OctaveShifterUI::refreshButtonColours()
{
    Colour plusButtonColour = plusButtonInitColour;
    Colour minusButtonColour = minusButtonInitColour;

    if(currentOctaveShift > 0)
    {
        plusButtonColour = plusButtonColour.brighter(currentOctaveShift / 2.0f);
        minusButtonColour = minusButtonColour.darker(currentOctaveShift / 2.0f);
    }
    else if(currentOctaveShift < 0)
    {
        plusButtonColour = plusButtonColour.darker(-currentOctaveShift / 2.0f);
        minusButtonColour = minusButtonColour.brighter(-currentOctaveShift / 2.0f);
    }

    plusButton.setColour(TextButton::buttonColourId, plusButtonColour);
    minusButton.setColour(TextButton::buttonColourId, minusButtonColour);
}

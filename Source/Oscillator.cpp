/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "Oscillator.h"
#include "Constants.h"

//==================================================================================
Oscillator::Oscillator(AudioProcessorValueTreeState * processorParameters)
 : parameters( processorParameters )
 , currentWaveShape( SINE )
{
    parameters->addParameterListener("waveShape", this);
}

//==================================================================================
Oscillator::~Oscillator()
{
    parameters->removeParameterListener("waveShape", this);
}

//==================================================================================
float Oscillator::renderWave(double currentPhase)
{
    switch( currentWaveShape )
    {
    case SINE:
        return renderSine(currentPhase);
    case SAW:
        return renderSaw(currentPhase);
    case TRIANGLE:
        return renderTriangle(currentPhase);
    case SQUARE:
        return renderSquare(currentPhase);
    }

    return 0.f;
}

//==================================================================================
String Oscillator::waveShapeToString(float waveShapeFloat)
{
	int waveShapeInt = static_cast<int>(waveShapeFloat);
	switch (waveShapeInt)
	{
	case 1:
		return "Sine";
	case 2:
		return "Saw";
	case 3:
		return "Triangle";
	case 4:
		return "Square";
	}
}

//==================================================================================
float Oscillator::renderSine(double currentPhase)
{
    return sin( currentPhase );
}

//==================================================================================
float Oscillator::renderSaw(double currentPhase)
{
    return 1.0f - (2.0f * currentPhase / TWO_PI);
}

//==================================================================================
float Oscillator::renderTriangle(double currentPhase)
{
    double value = -1.0 + (2.0 * currentPhase / TWO_PI);
    return 2.0 * ( fabs(value) - 0.5 );
}

//==================================================================================
float Oscillator::renderSquare(double currentPhase)
{
    if (currentPhase <= PI){
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
    return 0.f;
}

//==================================================================================
void Oscillator::parameterChanged(const String& parameterID, float newValue )
{
    int waveShapeInt = static_cast<int>(newValue);
    if(parameterID == "waveShape")
    {
        switch (waveShapeInt)
        {
        case 1:
            currentWaveShape = SINE;
            break;
        case 2:
            currentWaveShape = SAW;
            break;
        case 3:
            currentWaveShape = TRIANGLE;
            break;
        case 4:
            currentWaveShape = SQUARE;
            break;
        }
    }
}

/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "Oscillator.h"
#include "../Constants.h"

double Oscillator::sample_rate (0);

//==================================================================================
Oscillator::Oscillator(AudioProcessorValueTreeState * processorParameters, const String & waveShapeParamName)
 : parameters( processorParameters )
 , currentWaveShape( SINE )
 , waveShapeParameterName( waveShapeParamName )
 , currentAngle(0)
 , angleDelta(0)
 , frequency(0.0)
{
    parameters->addParameterListener( waveShapeParamName, this);
}

//==================================================================================
Oscillator::~Oscillator()
{
    parameters->removeParameterListener(waveShapeParameterName, this);
}

//==================================================================================
float Oscillator::renderWave(float angleInc)
{
    currentAngle += angleInc;
    currentAngleTick();

    switch( currentWaveShape )
    {
    case SINE:
        return renderSine();
    case SAW:
        return renderSaw();
    case TRIANGLE:
        return renderTriangle();
    case SQUARE:
        return renderSquare();
    }

    return 0.f;
}

//==================================================================================
void Oscillator::setFrequency(double frequency)
{
    this->frequency = frequency;
    angleDelta = TWO_PI * frequency / sample_rate;
}

//==================================================================================
double Oscillator::getFrequency() const
{
    return frequency;
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
float Oscillator::renderSine()
{
    return sin(currentAngle);
}

//==================================================================================
float Oscillator::renderSaw()
{
    return 1.0f - (2.0f * currentAngle / TWO_PI);
}

//==================================================================================
float Oscillator::renderTriangle()
{
    double value = -1.0 + (2.0 * currentAngle / TWO_PI);
    return 2.0 * ( fabs(value) - 0.5 );
}

//==================================================================================
float Oscillator::renderSquare()
{
    if (currentAngle <= PI){
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

//==================================================================================
void Oscillator::currentAngleTick()
{
    currentAngle += angleDelta;
    currentAngle = std::fmod(currentAngle, TWO_PI);
}

//==================================================================================
void Oscillator::parameterChanged(const String& parameterID, float newValue )
{
    int waveShapeInt = static_cast<int>(newValue);
    if(parameterID == waveShapeParameterName)
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

//==================================================================================
void Oscillator::setSampleRate(double sampleRate)
{
    sample_rate = sampleRate;
}

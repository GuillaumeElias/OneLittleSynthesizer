/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "EnvelopeUI.h"

#include "AbstractEnvelopeUI.h"

//==============================================================================
EnvelopeUI::EnvelopeUI(AudioProcessorValueTreeState& processorParameters)
    : parameters( processorParameters )
{
    //Attack slider
    attackAttachment = new SliderAttachment (parameters, "envAttack", attackSlider);
    attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    parameters.addParameterListener("envAttack", this);

    //Decay slider
    decayAttachment = new SliderAttachment (parameters, "envDecay", decaySlider);
    decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    parameters.addParameterListener("envDecay", this);

    //Sustain slider
    sustainAttachment = new SliderAttachment (parameters, "envSustain", sustainSlider);
    sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    parameters.addParameterListener("envSustain", this);

    //Sustain slider
    releaseAttachment = new SliderAttachment (parameters, "envRelease", releaseSlider);
    releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    parameters.addParameterListener("envRelease", this);

    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
}

//==============================================================================
EnvelopeUI::~EnvelopeUI()
{
    parameters.removeParameterListener("envAttack", this);
    parameters.removeParameterListener("envDecay", this);
    parameters.removeParameterListener("envSustain", this);
    parameters.removeParameterListener("envRelease", this);
}

//==============================================================================
void EnvelopeUI::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);

    Rectangle<int> bounds (getLocalBounds().reduced (10));

    g.drawFittedText ("Attack", bounds.removeFromLeft(50) , Justification::topLeft, 1);
    g.drawFittedText ("Decay", bounds.removeFromLeft(50) , Justification::topLeft, 1);
    g.drawFittedText ("Sustain", bounds.removeFromLeft(50) , Justification::topLeft, 1);
    g.drawFittedText ("Sweet release", bounds.removeFromLeft(50) , Justification::topLeft, 1);

    paintEnvelope(g);
}

//==============================================================================
void EnvelopeUI::paintEnvelope (Graphics& g)
{
    //PAINT ENVELOPE CONTOUR

    static const float edgeX = 220.f;
    static const float edgeY = 25.f;
    static const float height = 63.f;
    static const float sustainLength = 30.f;
    static const float bottomY = edgeY + height;
    float att = *parameters.getRawParameterValue("envAttack") / 30.f;
    float dec = *parameters.getRawParameterValue("envDecay") / 30.f;
    float sus = *parameters.getRawParameterValue("envSustain") * height;
    float rel = *parameters.getRawParameterValue("envRelease") / 30.f;

    Point<float> startPoint (edgeX, bottomY);
    Point<float> attackPoint(edgeX + att, edgeY);
    Point<float> decayPoint (attackPoint.getX() + dec, bottomY - sus);
    Point<float> sustainPoint(decayPoint.getX() + sustainLength, decayPoint.getY());
    Point<float> releasePoint(sustainPoint.getX() + rel, bottomY);

    g.setColour (Colours::white);
    g.drawLine( Line<float>(startPoint, attackPoint), 2.f);
    g.drawLine( Line<float>(attackPoint, decayPoint), 2.f);
    g.drawLine( Line<float>(decayPoint, sustainPoint), 2.f);
    g.drawLine( Line<float>(sustainPoint, releasePoint), 2.f);

    //PAINT PROGRESS INDICATOR

    for( const auto & envPair : envProgressMap )
    {
        EnvelopePhase curPhase = envPair.second.phase;
        const float curDeltaTime = envPair.second.deltaTime;
        const float curGain = envPair.second.gain;

        g.setColour ( VOICES_COLOURS[ envPair.first ] );

        switch( curPhase )
        {
            case ATTACK:
            {
                float posX = curDeltaTime * 33.33f; //1000 / 30

                g.drawLine(edgeX + posX, bottomY, edgeX + posX, bottomY - height * curGain, 1.f);

                break;
            }
            case DECAY:
            {
                float startX = edgeX + att;
                float posX = curDeltaTime * 33.33f;
                g.drawLine(startX + posX, bottomY, startX + posX, bottomY - height * curGain, 1.f);

                break;
            }

            case SUSTAIN:
            {
                float startX = edgeX + att + dec;
                g.drawLine(startX, bottomY, startX, bottomY - height * curGain, 1.f);
                break;
            }

            case RELEASE:
            {
                float startX = edgeX + att + dec + sustainLength;
                float posX = curDeltaTime * 33.33f;
                g.drawLine(startX + posX, edgeY + height, startX + posX, bottomY - height * curGain, 1.f);

                break;
            }
        }
    }
}

//==============================================================================
void EnvelopeUI::resized()
{
    attackSlider.setBounds(12, 18, 20, 72);
    decaySlider.setBounds(62, 18, 20, 72);
    sustainSlider.setBounds(112, 18, 20, 72);
    releaseSlider.setBounds(162, 18, 20, 72);
}

//==============================================================================
void EnvelopeUI::parameterChanged( const String& parameterID, float newValue )
{
    repaint();
}

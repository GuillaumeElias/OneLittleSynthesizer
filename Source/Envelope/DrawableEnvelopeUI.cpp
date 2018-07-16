/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelopeUI.h"

namespace
{
    const int PADDING_LEFT = 60;
}

//=================================================================
DrawableEnvelopeUIUpdater::DrawableEnvelopeUIUpdater(DrawableEnvelopeUI * envUI)
    : envelopeUI( envUI )
{
}

//=================================================================
void DrawableEnvelopeUIUpdater::handleAsyncUpdate()
{
    if(envelopeUI->isShowing())
    {
        envelopeUI->repaint();
    }
}

//=================================================================
DrawableEnvelopeUI::DrawableEnvelopeUI(AudioProcessorValueTreeState& processorParameters)
    : height( DRAWABLE_ENVELOPE_HEIGHT )
    , updater( this )
    , parameters( processorParameters )
{

    //Attack slider
    attackAttachment = new SliderAttachment (parameters, "filterAttack", attackSlider);
    attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );

    //Sustain slider
    sustainAttachment = new SliderAttachment (parameters, "filterSustain", sustainSlider);
    sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );

    //Release slider
    releaseAttachment = new SliderAttachment (parameters, "filterRelease", releaseSlider);
    releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );

    addAndMakeVisible(attackSlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
}

//=================================================================
void DrawableEnvelopeUI::paint(Graphics & g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);

    //PAINT ENVELOPE CONTOUR
    const std::vector<float> & attackValues = DrawableEnvelope::getValuesAttack();
    Point<float> pa(PADDING_LEFT, height - height * attackValues[0]);
    for( int i = 0; i < attackValues.size(); i++ )
    {
        Point<float> pb(PADDING_LEFT + DRAWABLE_ENVELOPE_BAR_WIDTH * i + DRAWABLE_ENVELOPE_BAR_WIDTH, height - height * attackValues[i]);
        g.drawLine( Line<float>( pa, pb ), 2.f );
        pa = pb;
    }

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
            case DECAY:
            {
                float val = curDeltaTime / DrawableEnvelope::getAttackTime() * DRAWABLE_ENVELOPE_NB_VALUES;
                float posX = PADDING_LEFT + val * DRAWABLE_ENVELOPE_BAR_WIDTH;

                g.drawLine(posX, height, posX, height * ( 1.f - curGain), 1.f);

                break;
            }

            case SUSTAIN:
            {
                //TODO
            }

            case RELEASE:
            {
                //TODO
            }
        }
    }

}

//==============================================================================
void DrawableEnvelopeUI::onEndNote( int voiceNumber )
{
    if( envProgressMap.find( voiceNumber ) != envProgressMap.end() )
    {
        if( envProgressMap[ voiceNumber ].phase != OFF )
        {
            updater.triggerAsyncUpdate();
        }
        envProgressMap[ voiceNumber ].phase = OFF;
        envProgressMap[ voiceNumber ].deltaTime = 0.f;
    }
}

//==============================================================================
void DrawableEnvelopeUI::onProgress(int voiceNumber, const EnvelopeProgress & progress)
{
    envProgressMap[voiceNumber] = {progress.phase, progress.deltaTime, progress.gain};

    updater.triggerAsyncUpdate();
}

//=================================================================
void DrawableEnvelopeUI::resized()
{
    attackSlider.setBounds(0, 18, 20, 72);
    sustainSlider.setBounds(20, 18, 20, 72);
    releaseSlider.setBounds(40, 18, 20, 72);
}


//=================================================================
void DrawableEnvelopeUI::mouseDown( const MouseEvent & event)
{
    handleClick(event.getMouseDownX(), event.getMouseDownY());
}

//=================================================================
void DrawableEnvelopeUI::mouseDrag(const MouseEvent & event)
{
    Point<int> position = event.getPosition();
    handleClick(position.getX(), position.getY());
}

//=================================================================
void DrawableEnvelopeUI::handleClick(int x, int y)
{
    x -= PADDING_LEFT;
    int index = roundToInt( x / DRAWABLE_ENVELOPE_BAR_WIDTH );
    if(index >= 0 && index < DRAWABLE_ENVELOPE_NB_VALUES && y > 0 && y <= height )
    {
        float value = 1.f - ((float)y / height);
        DrawableEnvelope::setValueAttack(index, value);
        updater.triggerAsyncUpdate();
    }
}

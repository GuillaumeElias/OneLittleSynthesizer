/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelopeUI.h"

namespace
{
    const int PADDING_LEFT_ATTACK = 120;
    const int PADDING_LEFT_SUSTAIN = PADDING_LEFT_ATTACK + DRAWABLE_ENVELOPE_BAR_WIDTH * DRAWABLE_ENVELOPE_NB_VALUES;
    const int PADDING_LEFT_RELEASE = PADDING_LEFT_SUSTAIN + DRAWABLE_ENVELOPE_SUSTAIN_WIDTH;
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
    //Filter env amount slider
    envAmountAttachment = new SliderAttachment (parameters, "filterEnvAmount", envAmountSlider);
    envAmountSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    envAmountSlider.setPopupDisplayEnabled (true, false, this);

    //Attack slider
    attackAttachment = new SliderAttachment (parameters, "filterAttack", attackSlider);
    attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    attackSlider.setPopupDisplayEnabled (true, false, this);

    addAndMakeVisible(envAmountSlider);
    addAndMakeVisible(attackSlider);
}

//=================================================================
void DrawableEnvelopeUI::paint(Graphics & g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);

    //labels
    Rectangle<int> bounds (getLocalBounds());
    g.drawFittedText ("Filter env amt", bounds.removeFromLeft(70), Justification::topLeft, 1);
    g.drawFittedText ("Attack time", bounds, Justification::topLeft, 1);

    //PAINT ENVELOPE CONTOUR
    //-attack-
    paintContour(g, DrawableEnvelope::getValuesAttack(), PADDING_LEFT_ATTACK);

    //-sustain bar and joint lines before and after-
    float sustainY = height - DrawableEnvelope::getSustainLevel() * height;
    g.drawLine(PADDING_LEFT_SUSTAIN, height - height * DrawableEnvelope::getValuesAttack()[DRAWABLE_ENVELOPE_NB_VALUES - 1], PADDING_LEFT_SUSTAIN, sustainY, 2.f);
    g.drawLine(PADDING_LEFT_SUSTAIN, sustainY, PADDING_LEFT_RELEASE, sustainY, 2.f);
    g.drawLine(PADDING_LEFT_RELEASE, sustainY, PADDING_LEFT_RELEASE, height - height * DrawableEnvelope::getValuesRelease()[0], 2.f);

    //-release-
    paintContour(g, DrawableEnvelope::getValuesRelease(), PADDING_LEFT_RELEASE);

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
                float xTime = curDeltaTime / DrawableEnvelope::getAttackTime() * DRAWABLE_ENVELOPE_NB_VALUES;
                float posX = PADDING_LEFT_ATTACK + xTime * DRAWABLE_ENVELOPE_BAR_WIDTH;

                g.drawLine(posX, height, posX, height * ( 1.f - curGain), 1.f);

                break;
            }

            case SUSTAIN:
            {
                g.drawLine(PADDING_LEFT_SUSTAIN, height, PADDING_LEFT_SUSTAIN, height * ( 1.f - curGain), 1.f);
                break;
            }

            case RELEASE:
            {
                float xTime = curDeltaTime / DrawableEnvelope::getReleaseTime() * DRAWABLE_ENVELOPE_NB_VALUES;
                float posX = PADDING_LEFT_RELEASE + xTime * DRAWABLE_ENVELOPE_BAR_WIDTH;

                g.drawLine(posX, height, posX, height * ( 1.f - curGain), 1.f);
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
    envAmountSlider.setBounds(20, 18, 20, 72);
    attackSlider.setBounds(80, 18, 20, 72);
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
    if(y <= 0 && y > height)
    {
        return;
    }

    float value = 1.f - ((float)y / height);

    bool release = false;
    if(x < PADDING_LEFT_SUSTAIN)            //ATTACK
    {
        x -= PADDING_LEFT_ATTACK;
    }
    else if ( x < PADDING_LEFT_RELEASE)     //SUSTAIN
    {
        DrawableEnvelope::setSustainLevel(value);

        updater.triggerAsyncUpdate();
        return;
    }
    else                                    //RELEASE
    {
        x -= PADDING_LEFT_RELEASE;
        release = true;
    }

    //set right value in vector based on x and y
    int index = roundToInt( x / DRAWABLE_ENVELOPE_BAR_WIDTH );
    if(index >= 0 && index < DRAWABLE_ENVELOPE_NB_VALUES )
    {
        if(release)
        {
            DrawableEnvelope::setValueRelease(index, value);
        }
        else
        {
            DrawableEnvelope::setValueAttack(index, value);
        }
        updater.triggerAsyncUpdate();
    }
}

//=================================================================
void DrawableEnvelopeUI::paintContour(Graphics & g, const std::vector<float> & values, int paddingLeft)
{
    Point<float> pa(paddingLeft, height - height * values[0]);
    for( int i = 0; i < values.size(); i++ )
    {
        Point<float> pb(paddingLeft + DRAWABLE_ENVELOPE_BAR_WIDTH * i + DRAWABLE_ENVELOPE_BAR_WIDTH, height - height * values[i]);
        g.drawLine( Line<float>( pa, pb ), 2.f );
        pa = pb;
    }
}


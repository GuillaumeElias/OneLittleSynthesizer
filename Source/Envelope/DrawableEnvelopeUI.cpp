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

    //==============================================================
    void setEnvelopeValue(int index, float value, bool release)
    {
        if(release)
        {
            DrawableEnvelope::setValueRelease(index, value);
        }
        else
        {
            DrawableEnvelope::setValueAttack(index, value);
        }
    }

}

//=================================================================
DrawableEnvelopeUI::DrawableEnvelopeUI(AudioProcessorValueTreeState& processorParameters)
    : height( DRAWABLE_ENVELOPE_HEIGHT )
    , parameters( processorParameters )
    , lastMouseDragIndex( -1 )
    , lastMouseDragValue( -1 )
{
    //Filter env amount slider
    envAmountAttachment = new SliderAttachment (parameters, "filterEnvAmount", envAmountSlider);
    envAmountSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    envAmountSlider.setPopupDisplayEnabled (true, false, this);

    //Attack slider
    attackAttachment = new SliderAttachment (parameters, "filterAttack", attackSlider);
    attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical );
    attackSlider.setPopupDisplayEnabled (true, false, this);

    //Loop button
    loopButtonAttachment = new ButtonAttachment(parameters, "loopDrawableEnvelope", loopButton);

    addAndMakeVisible(envAmountSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(loopButton);
}

//=================================================================
void DrawableEnvelopeUI::paint(Graphics & g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);

    //labels
    Rectangle<int> bounds (getLocalBounds());
    g.drawFittedText ("Filter env amt", bounds.removeFromLeft(75), Justification::topLeft, 1);
    g.drawFittedText ("Attack time", bounds.removeFromLeft(315), Justification::topLeft, 1);
    g.drawFittedText ("Loop", bounds.withTop(3), Justification::topLeft, 1);

    //PAINT ENVELOPE CONTOUR
    //-attack-
    paintContour(g, DrawableEnvelope::getValuesAttack(), PADDING_LEFT_ATTACK);

    //-sustain bar and joint lines before and after-
    float sustainY = height - DrawableEnvelope::getSustainLevel() * height;
    g.drawLine(PADDING_LEFT_SUSTAIN, height - height * DrawableEnvelope::getValuesAttack()[DRAWABLE_ENVELOPE_NB_VALUES - 1].get(), PADDING_LEFT_SUSTAIN, sustainY, 2.f);
    g.drawLine(PADDING_LEFT_SUSTAIN, sustainY, PADDING_LEFT_RELEASE, sustainY, 2.f);
    g.drawLine(PADDING_LEFT_RELEASE, sustainY, PADDING_LEFT_RELEASE, height - height * DrawableEnvelope::getValuesRelease()[0].get(), 2.f);

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

//=================================================================
void DrawableEnvelopeUI::resized()
{
    envAmountSlider.setBounds(20, 18, 20, 72);
    attackSlider.setBounds(86, 18, 20, 72);
    loopButton.setBounds(420, 0, 14, 14);
}

//=================================================================
void DrawableEnvelopeUI::mouseDown( const MouseEvent & event)
{
    handleClick(event.getMouseDownX(), event.getMouseDownY(), false);
}

//=================================================================
void DrawableEnvelopeUI::mouseDrag(const MouseEvent & event)
{
    Point<int> position = event.getPosition();
    handleClick(position.getX(), position.getY(), true);
}

//=================================================================
void DrawableEnvelopeUI::mouseUp( const MouseEvent &event)
{
    lastMouseDragIndex = -1;
    lastMouseDragValue = -1;
}

//=================================================================
void DrawableEnvelopeUI::handleClick(int x, int y, bool drag)
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
        if(drag)
        {
            fillAllPointsTillIndex(index, value, release);
        }
        else
        {
            setEnvelopeValue(index, value, release);
        }

        updater.triggerAsyncUpdate();
    }
}

//=================================================================
void DrawableEnvelopeUI::fillAllPointsTillIndex(int index, float value, bool release)
{
    if(lastMouseDragIndex == index)
    {
        return;
    }

    if(lastMouseDragIndex >= 0)
    {
        int diffIndexes = index - lastMouseDragIndex;
        float valueInc = (value - lastMouseDragValue) / abs(diffIndexes);
        float valueDelta = value;

        if(diffIndexes > 0) //drag right -> iterate towards left back to last index
        {
            for(int i=index; i > lastMouseDragIndex; i--)
            {
                setEnvelopeValue(i, valueDelta, release);
                valueDelta -= valueInc;
            }
        }
        else //drag left -> iterate towards right back to last index
        {
            for(int i=index; i < lastMouseDragIndex; i++)
            {
                setEnvelopeValue(i, valueDelta, release);
                valueDelta -= valueInc;
            }
        }

    }
    lastMouseDragIndex = index;
    lastMouseDragValue = value;
}

//=================================================================
void DrawableEnvelopeUI::paintContour(Graphics & g, const std::vector<Atomic<float>> & values, int paddingLeft)
{
    Point<float> pa(paddingLeft, height - height * values[0].get());
    for( int i = 0; i < values.size(); i++ )
    {
        Point<float> pb(paddingLeft + DRAWABLE_ENVELOPE_BAR_WIDTH * i + DRAWABLE_ENVELOPE_BAR_WIDTH, height - height * values[i].get());
        g.drawLine( Line<float>( pa, pb ), 2.f );
        pa = pb;
    }
}


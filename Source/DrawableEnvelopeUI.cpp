/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelopeUI.h"

//=================================================================
void DrawableEnvelopeUI::paint(Graphics & g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (10.0f);

    Rectangle<int> bounds (getLocalBounds().reduced (10));

}

//=================================================================
void DrawableEnvelopeUI::mouseDown( const MouseEvent & event)
{

}

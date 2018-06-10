/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DrawableEnvelopeUI : public Component, private MouseListener
{
    public:
        void paint(Graphics&) override;

    private:
        void mouseDown( const MouseEvent & event) override;


};


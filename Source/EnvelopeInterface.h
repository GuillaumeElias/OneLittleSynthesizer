/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

//==================================================================================

class EnvelopeListener
{
public:
    virtual void onEndNote() {};
    virtual void onSampleIncrement() {};
};

//==================================================================================

class EnvelopeInterface
{
public:

    virtual void noteOn() = 0;
    virtual void noteOff( bool allowTailOff ) = 0;
    virtual float computeGain() = 0;
};
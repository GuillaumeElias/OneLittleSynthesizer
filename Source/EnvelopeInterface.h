/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

//==================================================================================

enum EnvelopePhase { OFF, ATTACK, DECAY, SUSTAIN, RELEASE };

//==================================================================================
struct EnvelopeProgress
{
    EnvelopePhase phase;
    float deltaTime;
    float gain;
};

//==================================================================================

class EnvelopeListener
{
public:
    virtual void onEndNote( int voiceNb ) {};
    virtual void onProgress( int voiceNb, const EnvelopeProgress & progress ) {};
};

//==================================================================================

class EnvelopeInterface
{
public:

    virtual void noteOn() = 0;
    virtual void noteOff( bool allowTailOff ) = 0;
    virtual float computeGain() = 0;
};

/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Constants.h"

#include <map>

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
    virtual void onEndNote( int ) {};
    virtual void onProgress( int, const EnvelopeProgress &) {};
};

//==================================================================================

class AbstractEnvelope
{
public:

    AbstractEnvelope( int voiceNumber );

    virtual void noteOn() = 0;
    virtual void noteOff( bool allowTailOff ) = 0;
    virtual float computeGain() = 0;

    void addEnvelopeListener( EnvelopeListener * listener );
    void removeEnvelopeListener( EnvelopeListener * listener );

	const EnvelopeProgress & getEnvProgress();

protected:
    void notifyProgress( float deltaTime, float gain );
    void notifyEndNote();

    ListenerList<EnvelopeListener> listeners;
	CriticalSection listenersMutex;

    EnvelopePhase currentPhase, lastNotifyPhase;
    int voiceNumber;
    float lastDeltaTime;

	EnvelopeProgress envProgress;
};

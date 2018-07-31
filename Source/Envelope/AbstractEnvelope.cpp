/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "AbstractEnvelope.h"

AbstractEnvelope::AbstractEnvelope( int voiceNb )
 : voiceNumber( voiceNb )
 , currentPhase( OFF )
 , lastDeltaTime( 0.f )
{

}

//============================================================================
void AbstractEnvelope::notifyProgress(float deltaTime, float gain)
{
    //only notify listeners at ENV_PROGRESS_UPDATE_RATE when in the same phase
    float diffTime = deltaTime - lastDeltaTime;
    if( currentPhase == lastNotifyPhase && diffTime > 0 && diffTime < ENV_PROGRESS_UPDATE_RATE)
        return;

    EnvelopeProgress progress { currentPhase, deltaTime, gain };

    for( auto listener : listeners.getListeners() )
    {
        listener->onProgress(voiceNumber, progress );
    }

    lastDeltaTime = deltaTime;
    lastNotifyPhase = currentPhase;
}

//============================================================================
void AbstractEnvelope::notifyEndNote()
{
    for( auto listener : listeners.getListeners() )
    {
        listener->onEndNote( voiceNumber );
    }
}

//============================================================================
void AbstractEnvelope::addEnvelopeListener( EnvelopeListener * listener )
{
    listeners.add( listener );
}

//============================================================================
void AbstractEnvelope::removeEnvelopeListener( EnvelopeListener * listener )
{
    listeners.remove( listener );
}

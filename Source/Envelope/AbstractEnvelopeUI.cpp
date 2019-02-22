/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "AbstractEnvelopeUI.h"

//-----------------EnvelopeUIUpdater--------------------------------------------
EnvelopeUIUpdater::EnvelopeUIUpdater(AbstractEnvelopeUI * envUI)
    : envelopeUI(envUI)
{
}

//------------------------------------------------------------------------------
void EnvelopeUIUpdater::handleAsyncUpdate()
{
    if (envelopeUI->isShowing())
    {
        envelopeUI->repaint();
    }
}


//==================AbstractEnvelopeUI==========================================
AbstractEnvelopeUI::AbstractEnvelopeUI()
    : updater(this)
{

}

//==============================================================================
void AbstractEnvelopeUI::onEndNote(int voiceNumber)
{
    if (envProgressMap.find(voiceNumber) != envProgressMap.end())
    {
        if (envProgressMap[voiceNumber].phase != OFF)
        {
            updater.triggerAsyncUpdate();
        }
        envProgressMap[voiceNumber].phase = OFF;
        envProgressMap[voiceNumber].deltaTime = 0.f;
    }
}

//==============================================================================
void AbstractEnvelopeUI::onProgress(int voiceNumber, const EnvelopeProgress & progress)
{
    envProgressMap[voiceNumber] = { progress.phase, progress.deltaTime, progress.gain };

    updater.triggerAsyncUpdate();
}
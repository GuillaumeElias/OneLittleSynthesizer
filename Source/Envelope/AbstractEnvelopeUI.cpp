/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "AbstractEnvelopeUI.h"

 //==================AbstractEnvelopeUI==========================================
AbstractEnvelopeUI::AbstractEnvelopeUI()
{

}

//==============================================================================
void AbstractEnvelopeUI::addEnvelope(int voiceNumber, AbstractEnvelope * env)
{
	envelopesMap[voiceNumber] = env;
}


//==================EnvelopeUIUpdater===========================================
EnvelopeUIUpdater::EnvelopeUIUpdater(AbstractEnvelopeUI * envUI)
    : envelopeUI(envUI)
{
}

//==============================================================================
void EnvelopeUIUpdater::handleAsyncUpdate()
{
    if (envelopeUI->isShowing())
    {
        envelopeUI->repaint();
    }
}

//==============================================================================
void EnvelopeUIUpdater::onEndNote(int voiceNumber)
{
	triggerAsyncUpdate();
}

//==============================================================================
void EnvelopeUIUpdater::onProgress(int voiceNumber, const EnvelopeProgress & progress)
{
    triggerAsyncUpdate();
}
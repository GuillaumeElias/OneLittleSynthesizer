/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include <map>

#include "../JuceLibraryCode/JuceHeader.h"
#include "AbstractEnvelope.h"

class AbstractEnvelopeUI;

//----EnvelopeUIUpdater---------------------------------------------------------
class EnvelopeUIUpdater : public AsyncUpdater
{
public:
    EnvelopeUIUpdater(AbstractEnvelopeUI * envelopeUI);
    void handleAsyncUpdate() override;
private:
    AbstractEnvelopeUI * envelopeUI;
};


//====AbstractEnvelopeUI========================================================
class AbstractEnvelopeUI : public Component, public EnvelopeListener
{
public:
    AbstractEnvelopeUI();
    void onProgress(int voiceNumber, const EnvelopeProgress & progress) override;
    void onEndNote(int voiceNumber) override;

protected:
    std::map <int, EnvelopeProgress> envProgressMap;
    EnvelopeUIUpdater updater;
};

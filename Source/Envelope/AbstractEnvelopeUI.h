/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AbstractEnvelope.h"

 //====AbstractEnvelopeUI========================================================
class AbstractEnvelopeUI : public Component
{
public:
	AbstractEnvelopeUI();

	void addEnvelope(int voiceNumber, AbstractEnvelope * env);
protected:
	std::map<int, AbstractEnvelope *> envelopesMap;
};


//----EnvelopeUIUpdater---------------------------------------------------------
class EnvelopeUIUpdater : public AsyncUpdater, public EnvelopeListener
{
public:
    EnvelopeUIUpdater(AbstractEnvelopeUI * envelopeUI);
    void handleAsyncUpdate() override;

	void onProgress(int voiceNumber, const EnvelopeProgress & progress) override;
	void onEndNote(int voiceNumber) override;

private:
    AbstractEnvelopeUI * envelopeUI;
};



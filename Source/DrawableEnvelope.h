/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "EnvelopeInterface.h"

class DrawableEnvelope : public EnvelopeInterface
{
    public:
        DrawableEnvelope(AudioProcessorValueTreeState* processorParameters, double sampleRate);
        ~DrawableEnvelope();

        void noteOn() override;
        void noteOff(bool allowTailOff) override;
        float computeGain() override;

    private:
        std::vector<float> values;
        int valueIndex, sampleIndex;
        float totalTime;

        double sampleRate;

        //TODO bool repeat
};

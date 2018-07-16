/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Constants.h"
#include "AbstractEnvelope.h"

class DrawableEnvelope : public AbstractEnvelope
{
    public:
        DrawableEnvelope(AudioProcessorValueTreeState* processorParameters, double sampleRate, int voiceNumber);
        ~DrawableEnvelope();

        void noteOn() override;
        void noteOff(bool allowTailOff) override;
        float computeGain() override;

        //getters and setters for values shared across instances
        static const std::vector<float> & getValuesAttack();
        static const std::vector<float> & getValuesRelease();
        static float getSustainLevel();
        static float getAttackTime();
        static float getReleaseTime();
        static void setValueAttack(int index, float value);
        static void setValueRelease(int index, float value);
        static void setAttackTime(float attack);
        static void setSustainLevel(float sustain);
        static void setReleaseTime(float release);

    private:
        //those values are shared between all the envelope instances
        static std::vector<float> valuesAttack, valuesRelease;
        static float attackTime, releaseTime;
        static float sustainLevel;

        int valueIndex, sampleIndex;

        double sampleRate;

        AudioProcessorValueTreeState* parameters;
};

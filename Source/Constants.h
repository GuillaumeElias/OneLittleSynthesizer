/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

const double PI = 3.14159265358979323846;
const double TWO_PI = 2.0 * PI;

const float INIT_FILTER_FREQUENCY = 1000.0f; //Hz
const float MAX_FILTER_FREQUENCY = 2000.0f;
const float INIT_FILTER_RESONANCE = 0.59f;
const float INIT_WAVE_SHAPE = 1.f;
const float INIT_ENV_ATTACK = 100.f; //milliseconds
const float INIT_ENV_DECAY = 100.f;
const float INIT_ENV_SUSTAIN = 0.8f; //0->1
const float INIT_ENV_RELEASE = 100.0f;
const float ENV_PROGRESS_UPDATE_RATE = 0.01f; //in seconds
const double FILTER_SMOOTH_RAMP_LENGTH = 0.0001; //in seconds

const int NUMBER_OF_VOICES = 16;

const float INIT_FILTER_ENV_ATTACK = 400.f; //milliseconds
const float INIT_FILTER_ENV_RELEASE = 400.0f;
const int DRAWABLE_ENVELOPE_NB_VALUES = 32;
const int DRAWABLE_ENVELOPE_BAR_WIDTH = 5;
const float DRAWABLE_ENVELOPE_INIT_VALUES = 0.5f;
const int DRAWABLE_ENVELOPE_HEIGHT = 100;

const Array<Colour> VOICES_COLOURS =
{   //One colour per voice will be used for any voice-specific visualization
    Colour(0xffff0000), //red
    Colour(0xff228b22), //green
    Colour(0xff87cefa), //blue
    Colour(0xff8b4513), //brown
    Colour(0xffff1493), //pink
    Colour(0xfffafad2), //yellow
    Colour(0xff9370db), //purple
    Colour(0xff696969), //grey
    Colour(0xffff4500), //orange
    Colour(0xff2e8b57), //green2
    Colour(0xff6a5acd), //blue2
    Colour(0xfff4a460), //brown2
    Colour(0xffff69b4), //pink2
    Colour(0xffffffe0), //yellow2
    Colour(0xff663399), //purple2
    Colour(0xffffa500)  //orange2
};

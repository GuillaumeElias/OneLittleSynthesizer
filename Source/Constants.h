/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

const double PI = 3.14159265358979323846;
const double TWO_PI = 2.0 * PI;

const float INIT_FILTER_FREQUENCY = 1000.0f; //Hz
const float INIT_FILTER_RESONANCE = 0.59f;
const float INIT_WAVE_SHAPE = 1.f;
const float INIT_ENV_ATTACK = 100.f; //milliseconds
const float INIT_ENV_DECAY = 100.f;
const float INIT_ENV_SUSTAIN = 0.8f; //0->1
const float INIT_ENV_RELEASE = 100.0f;
const double FILTER_SMOOTH_RAMP_LENGTH = 0.0001; //in seconds

const int NUMBER_OF_VOICES = 16;

const int DRAWABLE_ENVELOPE_NB_VALUES = 32;
const float DRAWABLE_ENVELOPE_INIT_VALUES = 0.5f;

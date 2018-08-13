/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "PluginLookAndFeel.h"

//================================================================================
PluginLookAndFeel::PluginLookAndFeel()
{
    setColour (Slider::thumbColourId, Colours::white);
    setColour (Label::textColourId, Colours::white);
}

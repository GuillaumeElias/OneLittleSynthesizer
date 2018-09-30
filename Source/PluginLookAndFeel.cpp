/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "PluginLookAndFeel.h"

namespace
{
    const float MARGIN = 1.0f;
    const float TWO_MARGINS = 2 * MARGIN;
    const float HANDLE_THICKNESS = 5.0f;
    const int BORDER = 4;
    const int TWO_BORDERS = 2 * BORDER;
    const Typeface::Ptr MECHANICAL_TYPEFACE = Typeface::createSystemTypefaceFor(BinaryData::Mechanical_otf, BinaryData::Mechanical_otfSize);
    const Colour CHECKBOX_BACKGROUND_COLOUR = Colour(38, 50, 56);
}

//================================================================================
PluginLookAndFeel::PluginLookAndFeel()
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(MECHANICAL_TYPEFACE);
    setColour (Slider::thumbColourId, Colours::white);
    setColour (Label::textColourId, Colours::white);
}

//================================================================================
void PluginLookAndFeel::drawLinearSlider (Graphics & g, int x, int y, int width, int height,
                                          float sliderPos,
                                          float minSliderPos,
                                          float maxSliderPos,
                                          const Slider::SliderStyle style,
                                          Slider & slider)
{
    g.setColour(slider.findColour (Slider::backgroundColourId));
    g.fillRect(Rectangle<float> (x + MARGIN, y + MARGIN, width - TWO_MARGINS, height - TWO_MARGINS ));

    g.setColour(slider.findColour (Slider::thumbColourId));
    g.fillRect(slider.isHorizontal() ? Rectangle<float> (MARGIN + sliderPos - HANDLE_THICKNESS, y + MARGIN, HANDLE_THICKNESS, height - TWO_MARGINS)
                                      : Rectangle<float> (x + MARGIN, MARGIN + sliderPos - HANDLE_THICKNESS, width - TWO_MARGINS, HANDLE_THICKNESS));
}

//================================================================================
void PluginLookAndFeel::drawToggleButton (Graphics& g,
                                    ToggleButton& button,
                                    bool isMouseOverButton,
                                    bool isButtonDown)
{
    g.setColour(CHECKBOX_BACKGROUND_COLOUR);
    g.fillRect(0, 0, button.getWidth(), button.getHeight());

    if(button.getToggleState())
    {
        g.setColour(button.findColour (ToggleButton::tickColourId));
        g.fillRect(BORDER, BORDER, button.getWidth() - TWO_BORDERS, button.getHeight() - TWO_BORDERS);
    }
}

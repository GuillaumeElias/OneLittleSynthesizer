# OneLittleSynthesizer
This is my first attempt at making a synthesizer with the JUCE framework. The idea was to make a beginner-friendly synth with 2 oscillators (with cross-modulation capacity), 1 low-pass filter, 1 ADSR envelope as well as another hand-drawable envelope for controlling the filter cutoff. I wanted the user to be able to see the states of the notes he's playing within the envelope.

![You can see the three different notes being played](https://raw.githubusercontent.com/GuillaumeElias/OneLittleSynthesizer/master/OneLittleSynthesizer-screenshot.png)

# Binaries
Unfortunately I don't have the permission to distribute VST2 plugins so you will have to either build it yourself or send me a mail [mail](mailto:ipl.guillaume.elias@gmail.com?subject=[GitHub]OneLittleSynthesizer) to get the plugin in an executable form.

# How to build
## JUCE
This project is using JUCE 5.3.1. If you want to build it, you need to download it [here](https://github.com/WeAreROLI/JUCE/releases/tag/5.3.1). For the existing builds to work, you will need to put the SDK content under C:/JUCE or modifying the JUCE module paths with Projucer.

## VST
This project is using VST version 2.4 (but you can set another version using the Projucer if you want). You can download their latest SDK there: https://www.steinberg.net/en/company/developers.html

## Builds

Existing builds are Windows-only at the moment :

- **Builds/CodeBlocksWindows_win32/OneLittleSynthesizer.cbp**: 32 bit build for CodeBlocks using MinGW(GCC 6.3.3). You might run into one or two compiler issues while compiling with JUCE 5.3.0, but they are easily fixed by just adding the missing #include into the JUCE source file.

- **Builds/VisualStudio2017_win32/OneLittleSynthesizer.sln**: 32 bit build for VisualStudio 2017.

- **Builds/VisualStudio2017_x64/OneLittleSynthesizer.sln**: 64 bit build for VisualStudio 2017.

If you're not on Windows and/or want to build the project on another platform, just open *OneLittleSynthesizer.jucer* with Projucer and add a new exporter to fit your environement.

# Potential improvements:
- Improve CPU-usage efficiency
- Set OSC 2 frequency ratio to a scale that makes harmonic-sense (and that can snap by steps)
- VST3 export
- Presets

# License
This project is under GPLv3 license. See LICENSE file.

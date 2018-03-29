# OneLittleSynthesizer
This is a little synthesizer. A work in progress.

# How to build
## JUCE
This project is using JUCE 5.3.0. If you want to build it, you need to download it there: https://shop.juce.com/get-juce. You need to put the SDK content under C:/JUCE or modifying the JUCE module paths with Projucer.

## VST
This project is using VST version 2.4 (but you can set another version using the Projucer if you want). You can download their latest SDK there: https://www.steinberg.net/en/company/developers.html

## Builds

- **Builds/CodeBlocksWindows/OneLittleSynthesizer.cbp**: 32 bit build for CodeBlocks using MinGW(GCC 6.3.3). You might run into one or two compiler issues while compiling with JUCE 5.3.0, but they are easily fixed by just adding the missing #include into the JUCE source file.

- **Builds/VisualStudio2017/OneLittleSynthesizer.sln**: 64 bit build for VisualStudio 2017.

If you're not on Windows and/or want to build the project on another platform, just open *OneLittleSynthesizer.jucer* with Projucer and add a new exporter to fit your environement.

# License
This project is under GPLv3 license. See LICENSE file.

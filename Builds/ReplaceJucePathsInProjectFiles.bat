@echo off 

setlocal enableextensions disabledelayedexpansion

REM **[WINDOWS ONLY] This script replaces the paths to the JUCE directory in the VS and CodeBlocks files which are made relative by the Projucer**

REM Replace following according to the relative path the Projucer generated (check CodeBlocksWindows_win32\OneLittleSynthesizer.cbp)

set "juce_relative_path=../../../../../../../JUCE"
set "juce_absolute_path=C:/JUCE"

set "textFile=CodeBlocksWindows_win32\OneLittleSynthesizer.cbp"
call :replacePaths

REM Replace following according to the relative path the Projucer generated (check VisualStudio2017_win32\OneLittleSynthesizer_SharedCode.vcxproj)

set "juce_relative_path=..\..\..\..\..\..\..\JUCE"
set "juce_absolute_path=C:\JUCE"

set "textFile=VisualStudio2017_win32\OneLittleSynthesizer_SharedCode.vcxproj"
call :replacePaths

set "textFile=VisualStudio2017_win32\OneLittleSynthesizer_SharedCode.vcxproj.filters"
call :replacePaths

set "textFile=VisualStudio2017_x64\OneLittleSynthesizer_SharedCode.vcxproj"
call :replacePaths

set "textFile=VisualStudio2017_x64\OneLittleSynthesizer_SharedCode.vcxproj.filters"
call :replacePaths

EXIT /B %ERRORLEVEL%

:replacePaths
echo %textFile%

for /f "delims=" %%i in ('type "%textFile%" ^& break ^> "%textFile%" ') do (
	set "line=%%i"
	setlocal enabledelayedexpansion
	>>"%textFile%" echo(!line:%juce_relative_path%=%juce_absolute_path%!
	endlocal
)

EXIT /B 0
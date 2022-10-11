# residtest
Simple C++ test program to play a note writing Commodore64 SID register using libresid

This is a test for libresid, written by Felice Murolo (2022) linuxboy@REMOVEfel.hopto.org

NO COPYRIGHT ON THIS SOURCES. Share and modify like you want.

libresid is a library that can emulate MOS6581 (Commodore64 SID) and MOS8580 (newest Commodore64 SID)

original libresid source by Dag Lem: http://www.zimmers.net/anonftp/pub/cbm/crossplatform/emulators/resid/

CMake porting of libresid (Simon Owen): https://github.com/simonowen/resid

In this program, we emulate the C64 basic program that you see in the sources which play a musical note with his Attack,Decay,Sustain,Release.

residtest will play the note on pulseaudio device, standard audio device for linux distributions that use KDEPlasma as desktop.


# How to build
mkdir build

cd build

cmake ../

make

# Also in this repository
In this repository, you can find the file: "sid_frequencies.csv". In this file, you can found the value to set frequency register of voices of SID, to obtain the correct musical note.

# Thanks to:
Simon Owen for cmake port of libresid and for some good information on how to use the library

Dag Lem for libresid sources

Danny van Bruggen for sid_frequencies.csv



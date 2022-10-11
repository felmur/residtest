/*
 * residtest
 * This is a test for libresid, written by Felice Murolo (2022) linuxboy@REMOVEfel.hopto.org
 * NO COPYRIGHT ON THIS SOURCES. Share and modify like you want.
 *
 * libresid is a library that can emulate MOS6581 (Commodore64 SID) and MOS8580 (newest Commodore64 SID)
 * original libresid source by Dag Lem: http://www.zimmers.net/anonftp/pub/cbm/crossplatform/emulators/resid/
 * CMake porting of libresid (Simon Owen): https://github.com/simonowen/resid
 *
 * In this program, we emulate the C64 basic program that you see in the sources which play a musical note
 * with his Attack,Decay,Sustain,Release.
 *
 * residtest will play the note on pulseaudio device, standard audio device for linux distributions that
 * use KDEPlasma as desktop.
 *
 */



#include <pulse/simple.h>
#include <pulse/error.h>
#include <resid/sid.h>
#include <string.h>
#include <iostream>

using namespace std;


int main()
{
    int error=0,ret=0, i=0;
    pa_simple *s = NULL;
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };
    short * samples=NULL;
    cycle_count SID_CLOCK_PAL=985248;
    uint16_t SID_PAL_NUM_SAMPLES=882;


    // open pulseaudio
    if (!(s = pa_simple_new(NULL, "", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        cout << "pa_simple_new() failed" << endl;
        return -1;
    }

    // initialize SID
    SID *sid = new SID();
    sid->set_chip_model(MOS6581);
    sid->reset();
    sid->adjust_sampling_frequency(44100);

    /* THIS IS THE C64 BASIC PROGRAM TO EMULATE
    10 forx=54272to54296:poke x,0:next
    20 poke 54296,15
    30 poke54277,190
    40 poke54278,248
    50 poke54273,17:poke54272,103
    60 poke54276,17
    70 for x=1to250:next
    80 poke 54276,16
     */

    /* The SID registers are 32, from address 54272 to address 54303.
     * In libresid library, the start register 0 corresponds to address 54272(0xD400) SID register in C64 memory map.
     */

    // put 0 into the first 24 register
    for (i=0; i<24; i++) {
        sid->write(i,0);
    }
    // We will use voice #1
    // set volume to 15
    sid->write(24,15);
    // set some values for Attack-Decay-Sustain-Release
    sid->write(5,190);
    sid->write(6,248);
    // write value 4455 in 54272-54273, to set frequency of 261 Hz (C4 note)
    sid->write(0,103);
    sid->write(1,17);
    // select triangle waveform and enable the voice #1
    sid->write(4,17);

    samples = (short *) malloc(sizeof(short) * SID_PAL_NUM_SAMPLES);
    if (samples) {
        int j=0;
        while (i<100) {
            memset(samples, '\0',sizeof(short)*SID_PAL_NUM_SAMPLES);
            // read 882 short(2 bytes) values from SID
            sid->clock(SID_CLOCK_PAL, samples, SID_PAL_NUM_SAMPLES,1);
            // play sizeof(short)*882 values in pulseaudio device
            if (pa_simple_write(s, samples, (size_t) sizeof(short) * SID_PAL_NUM_SAMPLES, &error) < 0) {
                cout << "pa_simple_write() failed" << endl;
                ret = -1;
                goto finish;
            }
            for (j=0; j<1000000; j++);
            i++;
        }

        // clean pulseaudio device
        if (pa_simple_drain(s, &error) < 0) {
            cout << "pa_simple_drain() failed" << endl;
            ret = -1;
            goto finish;
        }
    }
    // disable the voice #1
    sid->write(0x04,16);

finish:
    if (s) pa_simple_free(s);
    if (samples) free(samples);
    return ret;
}

/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Generator / Modulation / Frequency
	Description:	Single-oscillator feedback frequency modulation.
*/

#include "../examples.h"

Sine<> osc(400);			// source sine
LFO<> mod(1./10);			// modulator of modulation index (0.1 hz)

void audioCB(AudioIOData& io){

	for(int i=0; i<io.framesPerBuffer(); ++i){

		float s = osc();
		io.out(0)[i] = io.out(1)[i] = s * 0.2f;
		
		osc.phaseAdd(s*0.01*mod.hann());
		//osc.freq((s*0.5+0.5)*mod.hann()*800 + 400);
	}
}

RUN(audioCB);
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fftw3.h>
#include "wav.hh"

using namespace std;

int		main(int argc, char *argv[])
{
    Wav*		   wav = new Wav("ane.wav");
	fftw_complex*  out;
	fftw_plan p;

	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * wav->data_length_get ());
	p = fftw_plan_dft_r2c_1d(wav->data_length_get (), wav->data_get (), out, FFTW_ESTIMATE);

	fftw_execute(p);
	
	for (int i = 0; i < wav->data_length_get () / 2 + 1; ++i)
		std::cout << i << " " << sqrt (out[i][0] * out[i][0] + out[i][1] * out[i][1]) << " # " << out[i][0] << " + " << out[i][1] << "i" << std::endl;

    return EXIT_SUCCESS;
}

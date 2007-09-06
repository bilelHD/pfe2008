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

	out = wav->compute_fft (0);
	
	for (int i = 0; i < wav->split_length_get (0) / 2 + 1; ++i)
		std::cout << i * (double) wav->header_get().nSamplesPerSec / (double) wav->split_length_get (0) << " " << sqrt (out[i][0] * out[i][0] + out[i][1] * out[i][1]) << " # " << out[i][0] << " + " << out[i][1] << "i" << std::endl;

    return EXIT_SUCCESS;
}

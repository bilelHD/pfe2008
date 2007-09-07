#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fftw3.h>
#include "wav.hh"

using namespace std;

int		main(int argc, char *argv[])
{
    Wav*		   wav = new Wav("ane.wav");
    int             split_index = 0;    

	fftw_complex*  out;

	out = wav->compute_fft (split_index);
    wav->compute_mel_scale (out, split_index);
    wav->apply_all_filters (out, split_index);
	
	//for (int i = 0; i < wav->split_length_get (split_index) / 2 + 1; ++i)
	//	std::cout << i /* * (double) wav->header_get().nSamplesPerSec / (double) wav->split_length_get (split_index)*/ << " " << sqrt (out[i][0] * out[i][0] + out[i][1] * out[i][1]) << " # " << out[i][0] << " + " << out[i][1] << "i" << std::endl;

    return EXIT_SUCCESS;
}

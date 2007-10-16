#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fftw3.h>
#include "wav.hh"

using namespace std;

int		main(int argc, char *argv[])
{
    Wav*		   wav = new Wav("ane.wav");
    int             split_index = 61;

	fftw_complex*  out;

	out = wav->compute_fft2 (split_index);

    v_int_int&  indexes = wav->get_filters_bank_params ();
    v_double&   mel_coeffs = wav->tri_band_filters (out, *(indexes[0]), *(indexes[1]), *(indexes[2]));
    v_double&   coeffs = wav->compute_dct2 (mel_coeffs);

    for (int i = 0; i < coeffs.size (); ++i)
	{
		std::cout << i << " " << coeffs[i] << std::endl;
	}

    /*for (int i = 0; i < indexes[0]->size (); ++i)
    {
        std::cout << (i + 1) << " start = " << (*indexes[0])[i] << ", center = " << (*indexes[1])[i] << ", stop = " << (*indexes[2])[i] << std::endl;
    }*/

    /*for (int i = 0; i < mel_coeffs.size (); ++i)
	{
		std::cout << (i + 1) << " " << mel_coeffs[i] << std::endl;
	}*/

    //wav->compute_mel_scale (out, split_index);
    //v_double* mel_coeffs = wav->apply_all_filters (out, split_index);
    //v_double& coeffs = wav->compute_dct2(*mel_coeffs);
	
	/*for (int i = 0; i < coeffs.size (); ++i)
	{
		std::cout << i << " " << coeffs[i] << out[i][1]<< std::endl;
	}*/
	//for (int i = 0; i < wav->split_length_get (split_index) / 2 + 1; ++i)
	//	std::cout << i /* * (double) wav->header_get().nSamplesPerSec / (double) wav->split_length_get (split_index)*/ << " " << sqrt (out[i][0] * out[i][0] + out[i][1] * out[i][1]) << " # " << out[i][0] << " + " << out[i][1] << "i" << std::endl;

    return EXIT_SUCCESS;
}

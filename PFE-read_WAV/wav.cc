#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "wav.hh"
#include "define.hh"
/*
** Constructor
*/
Wav::Wav(std::string filename)
{
    FILE*           file;
    short*          data_short = 0;
    int*         	data_int = 0;
    unsigned char*  data_char = 0;
    unsigned int	max_amplitude;
 
    if (!(file = fopen(filename.c_str (), "rb")))
    {
	   std::cerr << "Ouverture du fichier " << filename << " impossible." << std::endl;
	   return;
    }
    fread(&header_, 1, sizeof (s_header), file);

    // 8 bits (= 1 byte) per sample
    if (header_.nBitsPerSample == 8)
    {
        data_length_ = header_.dLen;
        data_char = new unsigned char[data_length_];
        data_ = new double[data_length_];
  
        // Storing data from file
        fread(data_char, 1, data_length_, file);
        max_amplitude = (unsigned int) pow(2, 8) - 1;
        for (int i = 0; i < data_length_; ++i)
            data_[i] = (double) (data_char[i]) / max_amplitude;
    }
    // 16 bits (= 2 bytes) per sample
    else if (header_.nBitsPerSample == 16)
    {
        data_length_ = header_.dLen / 2;
        data_short = new short[data_length_];
        data_ = new double[data_length_];

        // Storing data from file
        fread(data_short, 2, data_length_, file);
        max_amplitude = (unsigned int) pow(2, 15);
        for (int i = 0; i < data_length_; ++i)
        {
            data_[i] = (double) (data_short[i]) / max_amplitude;
			std::cout << data_[i] << std::endl;
		}
	}
    // 32 bits (= 4 bytes) per sample
    else if (header_.nBitsPerSample == 32)
    {
        data_length_ = header_.dLen / 4;
        data_int = new int[data_length_];
        data_ = new double[data_length_];

        // Storing data from file
        fread(data_int, 4, data_length_, file);
        max_amplitude = (unsigned int) pow(2, 31);
        for (int i = 0; i < data_length_; ++i)
            data_[i] = (double) (data_int[i]) / max_amplitude;
    }

	// compute splits information
	double  sample_duration_ms = (double) CONVERT_SEC_TO_MSEC(1) / header_.nSamplesPerSec;

	/* initial computation formula: 
    ** samples_per_split_ = (int) (SPLIT_TIME_MS / sample_duration_ms);
    ** nb_splits_ = data_length_ / samples_per_split_;
    */
    samples_per_split_ = (int) ((double) header_.nSamplesPerSec / ((double) 8000 / 256));
    overlap_ = (int) round ((double) samples_per_split_ / 3);
    nb_splits_ = (int) floor ((data_length_ - overlap_) / (samples_per_split_ - overlap_));

	fclose(file);
}

/*
** Class Wav
*/
Wav::~Wav()
{
    if (data_)
        delete(data_);
}

/*
** Wav::split_length_get
*/
int Wav::split_length_get (int split_index)
{
	if (split_index == nb_splits_ - 1)
	    return samples_per_split_ + (data_length_ % samples_per_split_);
	return samples_per_split_;
}

/*
** Wav::start_split_get
*/
int Wav::start_split_get (int split_index)
{
    int     step = samples_per_split_ - overlap_;
    
    return split_index * step;
}

/*
** Wav::compute_caracteristics_vectors
*/
void	Wav::compute_caracteristics_vectors ()
{

}

/*
**
*/
fftw_complex*	Wav::compute_fft(int split_index)
{
    fftw_complex*	out;
    fftw_plan 		p;
    int            split_length = split_length_get (split_index);
    
	out = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * split_length);
	p = fftw_plan_dft_r2c_1d (split_length, data_ + split_index * samples_per_split_, out, FFTW_ESTIMATE);

	fftw_execute(p);
	return out;
}

/*
**
*/
fftw_complex*	Wav::compute_fft2 (int split_index)
{
    fftw_complex*	out;
    fftw_plan 		p;
    int             start_split = start_split_get (split_index);
    int             split_length = samples_per_split_;
    
	out = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * split_length);
	p = fftw_plan_dft_r2c_1d (split_length, data_ + start_split, out, FFTW_ESTIMATE);
	
    fftw_execute(p);

    for (int i = 0; i < split_length; ++i)
        out[i][0] = sqrt (pow (out[i][0], 2) + pow (out[i][1], 2));
	return out;
}

/*
** Wav::compute_mel_scale
*/
void	Wav::compute_mel_scale (fftw_complex* data, int split_index)
{
	double     f;
    int        split_length = split_length_get (split_index);
    
	for (int i = 0; i < split_length; ++i)
	{
		f = sqrt (data[i][0] * data[i][0] + data[i][1] * data[i][1]);
	    data[i][0] = 2595 * log10 (1 + (f / 700));
	}
}

/*
** Wav::interpole
*/
double     Wav::interpole (fftw_complex* data, double x)
{
    return (data[(int) ceil (x)][0] + data[(int) floor (x)][0]) * (x - floor (x));
}

/*
** Wav::apply_filter
*/
double  Wav::apply_filter (fftw_complex* data, int split_index, double x_start, double height, double width, double end_index)
{
    int     split_length = split_length_get (split_index);
    double  second_length = width;
    double  sum = 0;

    // The current filter is the last one
    if (x_start + width + STEP_FILTER + 2 * (width + STEP_FILTER) > end_index)
        second_length = end_index - (x_start + width);

    // First half of area
    for (double i = x_start; i < x_start + width; ++i)
        sum += (height * i / width) * interpole (data, i);

    // Second half of area
    for (double i = 0; i < second_length; ++i)
        sum += (height * i / second_length) * interpole (data, x_start + width + i);

    /*std::cout << x_start << " 0" << std::endl;
    std::cout << x_start + width << " " << height << " # width=" << width << std::endl;
    std::cout << x_start + width + second_length << " 0" << std::endl;*/

    return sum;
}

/*
** Wav::apply_all_filters
*/
v_double*   Wav::apply_all_filters (fftw_complex* data, int split_index)
{
    int         split_length = split_length_get (split_index);
    v_double*   res = new v_double;
    double      coeff = (double) header_get().nSamplesPerSec / (double) split_length_get (split_index);
    int         start_index = START_FREQ / (int) coeff;
    int         end_index = END_FREQ / (int) coeff;
    double      width = (end_index - start_index) / (NB_FILTERS + 1) - STEP_FILTER * ((double) (NB_FILTERS)  / 2);

    for (double i = start_index; i + 2 * width < end_index; i += width)
    {
        res->insert (res->end (), apply_filter (data, split_index, i, FILTER_HEIGHT, width, end_index));
        width += STEP_FILTER;
    }
    return res;        
}

/*
**
*/
v_double&    Wav::compute_dct(v_double &mel_coeffs)
{
	v_double*   res = new v_double;
	int         N = mel_coeffs.size ();

	for (int k = 0; k < N; ++k)
	{
		double  res_k = 0;
		
		for (int n = 0; n < N; ++n)
		{
			double	w_n = (n == 0) ? sqrt ((double) 1 / N) : sqrt ((double) 2 / N);
			double	cos_nk = cos ((PI * (2 * (n + 1) - 1) * ((k + 1) - 1)) / (2 * N));

			res_k += w_n * mel_coeffs[n] * cos_nk;
		}
		(*res).push_back(res_k);
	}
	return *res;
}

/*
**
*/
v_double&    Wav::compute_dct2(v_double &mel_coeffs)
{
	v_double*   res = new v_double;
	int         N = mel_coeffs.size ();

	for (int k = 0; k < N; ++k)
	{
		double  res_k = 0;

		for (int n = 0; n < N; ++n)
		{
			double	w_n = sqrt ((double) 2 / N);
			double	cos_nk = cos ((PI * ((n + 1) - 0.5) * (k + 1)) / N);

			res_k += w_n * mel_coeffs[n] * cos_nk;
		}
		(*res).push_back(res_k);
	}
	return *res;
}

/*
** tri_band_filters: computes tri-band filters
*/
v_double&   Wav::tri_band_filters (fftw_complex* fftmag, v_int& start, v_int& center, v_int& end)
{   
    v_double*   res = new v_double;
    v_double*   filtermag = new v_double;
    int         i;
    int         j;

    for (i = 0; i < NB_FILTERS; ++i)
    {
        double  sum = 0;

        filtermag->clear ();
        for (j = start[i]; j <= center[i]; ++j)
            filtermag->push_back (((double) j - start[i]) / ((double) center[i] - start[i]));
        
        for (j = center[i] + 1; j <= end[i]; ++j)
            filtermag->push_back (1 - ((double) j - center[i]) / ((double) end[i] - center[i]));


        for (j = start[i]; j <= end[i]; ++j)
            sum += fftmag[j - 1][0] * (*filtermag)[j - start[i]];
        res->push_back (sum);
    }
    delete (filtermag);
    return *res;
}

/*
** mel_to_freq
*/
double      Wav::mel_to_freq (double mel)
{
    return 700 * (pow (10, (mel / 2595)) - 1);
}

/*
** freq_to_mel
*/
double      Wav::freq_to_mel (double freq)
{
    return 2595 * log10 (1 + freq / 700);
}

/*
** get_filters_bank_params: computes split indexes for each tri-band filter
*/
v_int_int&  Wav::get_filters_bank_params ()
{
    v_int_int*  res = new v_int_int;
    double      max_mel_freq = freq_to_mel (header_.nSamplesPerSec / 2);
    double      side_width = max_mel_freq / (NB_FILTERS + 1);

    res->push_back (new v_int);
    res->push_back (new v_int);
    res->push_back (new v_int);
    for (int i = 0; i < NB_FILTERS; ++i)
        for (int j = i; j < i + 3; ++j)
            (*res)[j - i]->push_back (1 + (int) floor (mel_to_freq (j * side_width) / header_.nSamplesPerSec * samples_per_split_));
    return *res;    
}

/*
** little_to_big: conversion from little endian to big endian for SHORT
*/
short Wav::little_to_big(short i)
{
    u_short_value c;
 
    c.short_value = i;
    char    temp;
 
    temp = c.char_value[0];
    c.char_value[0] = c.char_value[1];
    c.char_value[1] = temp;

    return c.short_value;
}

/*
** little_to_big: conversion from little endian to big endian for INT
*/
int Wav::little_to_big(int i)
{
    u_int_value c;

    c.int_value = i;
    char    temp;

    temp = c.char_value[0];
    c.char_value[0] = c.char_value[1];
    c.char_value[1] = temp;
 
    temp = c.char_value[3];
    c.char_value[3] = c.char_value[4];
    c.char_value[4] = temp;

    return c.int_value;
}

/*
** Displays header data
*/
std::ostream& operator<<(std::ostream& out, s_header& h)
{
    char          temp[5];

    temp[4] = 0;
    std::cout << "Header:" << std::endl << "------------------------" << std::endl;

    std::cout << "rID: " << strncpy(temp, h.rID, 4) << std::endl
              << "rLen: " << h.rLen << std::endl;
            
    std::cout << "wID: " << strncpy(temp, h.wID, 4) << std::endl;
    std::cout << "fId: " << strncpy(temp, h.fId, 4) << std::endl
 		      << "fLen: " << h.fLen << std::endl
		      << "wFormatTag: " << h.wFormatTag << std::endl
		      << "nChannels: " << h.nChannels << std::endl
		      << "nSamplesPerSec: " << h.nSamplesPerSec << std::endl
		      << "nAvgBytesPerSec: " << h.nAvgBytesPerSec << std::endl
		      << "nBlockAlign: " << h.nBlockAlign << std::endl
		      << "nBitsPerSample: " << h.nBitsPerSample << std::endl;
    std::cout << "dId: " << strncpy(temp, h.dId, 4) << std::endl
		      << "dLen: " << h.dLen << std::endl;
}

/*
** Displays wav data
*/
std::ostream& operator<<(std::ostream& out, Wav& wav)
{
    for (int i = 0; i < wav.data_length_get (); ++i)
        	std::cout << i << " " << wav.data_get()[i] << std::endl;
}

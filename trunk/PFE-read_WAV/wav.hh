#ifndef WAV_HH_
# define WAV_HH_

# include <iostream>
# include <string>
# include <fftw3.h>
# include "types.hh"

/*
** PCM format from: http://www.freesoundeditor.com/incagen.html?docwave.htm~main
*/

/*
** Unions for conversion from little to big endian
*/
union u_short_value
{
     short       		short_value;
     unsigned char      char_value[2];
};
union u_int_value
{
     int       			int_value;
     unsigned char      char_value[4];
};

/*
** Structure s_header
*/
struct  			s_header
{
    char      		rID[4];
    int       		rLen ;
    char      		wID[4];
 
    char      		fId[4];
    int       		fLen;
    unsigned short  wFormatTag;
    unsigned short  nChannels;
    int				nSamplesPerSec;
    int             nAvgBytesPerSec;
    unsigned short  nBlockAlign;
    unsigned short  nBitsPerSample;

    char     		dId[4];
    int       		dLen;
};

/*
** Class Wav
*/
class	Wav
{
 public:
		Wav(std::string filename);
        ~Wav();

 		short 			little_to_big (short i);
		int 			little_to_big (int i);

		s_header&       header_get() { return header_; }
		double*			data_get() { return data_; }
		int				nb_splits_get () { return nb_splits_; };
		int             samples_per_split_get () { return samples_per_split_; };
		int				data_length_get() { return data_length_; }

		int             split_length_get (int split_index);
		
		void            compute_caracteristics_vectors();
		fftw_complex*	compute_fft(int split_index);
		void            compute_mel_scale(fftw_complex* data, int split_index);
		double       	apply_filter (fftw_complex* data, int split_index, double x_start, double height, double width, double end_index);
		v_double*       apply_all_filters (fftw_complex* data, int split_index);
        double          interpole (fftw_complex* data, double x);
        v_double&		compute_dct(v_double &mel_coeffs);
        v_double&    	compute_dct2(v_double &mel_coeffs);
		


 private:
		s_header       						header_;
		int            						data_length_;
		double*		   						data_;
		std::vector<v_double>				cv_;
		int                                 nb_splits_;
		int                                 samples_per_split_;
};

std::ostream& operator<<(std::ostream& out, Wav& wav);

#endif /* !WAV_HH_ */

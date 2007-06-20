#ifndef WAV_HH_
# define WAV_HH_

# include <iostream>
# include <string>

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
 char      			rID[4];
 int       			rLen ;
 char      			wID[4];
 
 char      			fId[4];
 int       			fLen;
 unsigned short     wFormatTag;
 unsigned short     nChannels;
 int				nSamplesPerSec;
 int                nAvgBytesPerSec;
 unsigned short     nBlockAlign;
 unsigned short     nBitsPerSample;

 char     			dId[4];
 int       			dLen;
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
		int				data_length_get() { return data_length_; }

 private:
		s_header       header_;
		int            data_length_;
		double*		   data_;
};

std::ostream& operator<<(std::ostream& out, Wav& wav);

#endif /* !WAV_HH_ */

#include "wav.hh"
#include <string.h>
#include <stdlib.h>

/*
** Constructor
*/
Wav::Wav(std::string    filename)
{
 FILE*                  file;
 short*                 data_short = 0;
 int*         			data_int = 0;
 unsigned char*         data_char = 0;
 
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
  for (int i = 0; i < data_length_; ++i)
  	  data_[i] = (double) (data_char[i]) / 255;
 }
 // 16 bits (= 2 bytes) per sample
 else if (header_.nBitsPerSample == 16)
 {
  data_length_ = header_.dLen / 2;
  data_short = new short[data_length_];
  data_ = new double[data_length_];

  // Storing data from file
  std::cout << fread(data_short, 2, data_length_, file) << std::endl;
  for (int i = 0; i < data_length_; ++i)
  	  data_[i] = (double) (data_short[i]) / 32768;
 }
 // 32 bits (= 4 bytes) per sample
 else if (header_.nBitsPerSample == 32)
 {
  data_length_ = header_.dLen / 4;
  data_int = new int[data_length_];
  data_ = new double[data_length_];

  // Storing data from file
  fread(data_int, 4, data_length_, file);
  for (int i = 0; i < data_length_; ++i)
  	  data_[i] = (double) (data_int[i]) / 2147483648;
 }

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

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define RIFF_4CC_ID (0x46464952) // "RIFF" little endian
#define WAVE_4CC_ID (0x45564157) // "WAVE" little endian
#define FMT_4CC_ID  (0x20746d66) // "fmt " little endian 
#define DATA_4CC_ID (0x61746164) // "data" little endian 
#define CUE_4CC_ID  (0x20657563) // "cue " little endian

struct t_audio_data
{ 
	 //"data" 4-character code
	 unsigned long   Data4CC; 

	 //chunkSize is the number of bytes in the chunk, 
	 //not counting the 8 bytes used by ID and Size fields nor any possible 
	 //pad byte needed to make the chunk an even size (ie, chunkSize is the 
	 //number of remaining bytes in the chunk after the chunkSize field, 
	 //not counting any trailing pad byte).
	 unsigned long   chunkSize;
};

struct t_wav_hdr 
{
     unsigned long   Riff4CC;	 // "RIFF" 4-character code    
     unsigned long   FileSize;	 // total file size in bytes 
     unsigned long   Wave4CC;	 // "WAVE" 4-character code      
     unsigned long   Fmt4CC;	 // "fmt " 4-character code     
     unsigned long	 FormatSize; // wave format size in bytes   

	 //Waveform-audio format type
	 //A complete list of format tags can be found in the Mmreg.h header file. 
	 //For one- or two-channel PCM data, this value should be WAVE_FORMAT_PCM (1)
     unsigned short  FormatTag;	

	 //Number of channels in the waveform-audio data
	 //Mono data uses one channel and stereo data uses two channels
     unsigned short	 nbChannels;	

	 //Sample rate, in samples per second Hz
     unsigned long   nbSamplesPerSec;	

	 //Required average data transfer rate, in bytes per second. 
	 //For example, 16-bit stereo at 44.1 kHz has an average data rate 
	 //of 176,400 bytes per second (2 channels — 2 bytes per sample 
	 //per channel — 44,100 samples per second).
     unsigned long   nbAvgBytesPerSec;

	 //Block alignment, in bytes. The block alignment is the minimum atomic unit of data. 
	 //For PCM data, the block alignment is the number of bytes used by a single sample, 
	 //including data for both channels if the data is stereo. 
	 //For example, the block alignment for 16-bit stereo PCM 
	 //is 4 bytes (2 channels — 2 bytes per sample).
     unsigned short  blockAlign;	

	 //Number of bits per sample
     unsigned short  bitsPerSample;	

};

struct t_cue_chunk
{
	char dwName[4];
	int position;
	unsigned int chunk;
	int chunkStart;
	int blockStart;
	int sampleOffset;
};

struct t_cue_hdr
{
  unsigned long Cue4CC; // "cue " 4-character code  
  long chunkSize;
  long dwCuePoints;
};

struct t_point {short x, y;};
struct t_Size {short x, y;};

struct t_data
{
	int posInFile;
	t_point point;
	t_Size size;
	short numImg;
	float time;
};
float* getMarker(ifstream& wavefile, int& nbcue);
unsigned short* getRawAudioData(ifstream& wavefile, unsigned long& bufferSize);
#include "main.h"

int main(int argc, char* argv[])
{
	//ifstream wavefile("die2.wav", ios::binary);

	//Get Audio Raw Data
	//unsigned long rawDataBufferSize = 0;
	//unsigned short* rawAudioData = getRawAudioData(wavefile, rawDataBufferSize);

	//Get Cue from Audio
	int nb_cue = 22;
	//float* time_cue = getMarker(wavefile, nb_cue);
	//wavefile.close();

	float cues[22] = 
	{
		48558,   
		200119,  
		320780,  
		467927,  
		525565,  
		601831,  
		698948,  
		837266,  
		919668,  
		1058400, 
		1137446, 
		1200719, 
		1333151, 
		1380238, 
		1464112, 
		1543500, 
		1629581, 
		1695133, 
		1753418, 
		1808100, 
		1989427, 
		2166003
	};

	for (int i = 0; i < 22; i++)
	{
		cues[i] /= 44100.0;
	}

	float* time_cue = cues;

	ifstream nbRectanglePoints("points.txt");
	if(nbRectanglePoints.fail())
	{
		cout << "Error : can't read points.txt" << endl;
		return 0;
	}

	int nbRectangle = 0;
	nbRectanglePoints >> nbRectangle;

	t_data* data;

	if(nb_cue == nbRectangle)
	{
		data = new t_data[nb_cue];

		for(int i = 0; i < nb_cue; i++)
		{
			nbRectanglePoints >> data[i].posInFile;
			nbRectanglePoints >> data[i].numImg;
			nbRectanglePoints >> data[i].point.x;
			nbRectanglePoints >> data[i].point.y;
			nbRectanglePoints >> data[i].size.x;
			nbRectanglePoints >> data[i].size.y;
			data[i].time = time_cue[i];

			std::cout << "Time cue : " << data[i].time << std::endl;
		}

		nbRectanglePoints.close();

		ofstream outputData("output.data", ios::binary);
		outputData.seekp(0);

		//Save Raw Audio Data
		//outputData.write((char*)&rawDataBufferSize, sizeof(rawDataBufferSize)); 
		//outputData.write((char*)rawAudioData, rawDataBufferSize);

		//Markers and Points Data
		outputData.write((char*)&nb_cue, sizeof(int));
		outputData.write((char*)data, sizeof(t_data) * nb_cue);
	
		outputData.close();
	}

	delete[] data;

	//system("pause");
	return 0;
}
unsigned short* getRawAudioData(ifstream& wavefile, unsigned long& bufferSize)
{
	t_wav_hdr wavHdr; 
	wavefile.read((char*)&wavHdr, sizeof(t_wav_hdr));
	
	//Two Bytes Decay ??????
	{short decay; wavefile.read((char*)&decay, sizeof(short));}
	
	t_audio_data audioData;
	audioData.Data4CC = 0;
	while(audioData.Data4CC != DATA_4CC_ID && !wavefile.eof())
		wavefile.read((char*)&audioData, sizeof(t_audio_data));
	
	unsigned short* audioFrames = new unsigned short[audioData.chunkSize/2];
	wavefile.read((char*)audioFrames, audioData.chunkSize);

	bufferSize = audioData.chunkSize;
	return audioFrames;
}
 float* getMarker(ifstream& wavefile, int& nbCue)
{
	t_cue_hdr cueHdr;
	cueHdr.Cue4CC = 0;
	while(cueHdr.Cue4CC != CUE_4CC_ID && !wavefile.eof()) 
		wavefile.read((char*) &cueHdr, sizeof(t_cue_hdr));

	cout << hex << cueHdr.Cue4CC << endl;

	nbCue = cueHdr.dwCuePoints;
	t_cue_chunk* cue_chunk = new t_cue_chunk[nbCue];
	wavefile.read((char*)cue_chunk, sizeof(t_cue_chunk) * nbCue);

	float* timeCue = new float[nbCue];
	float oneOverSR = 1.0 / 44100;
	float* cue = timeCue; 
	t_cue_chunk* chunk = cue_chunk;
	while(nbCue--) *cue++ = (*chunk++).position * oneOverSR; 

	nbCue = cueHdr.dwCuePoints;
	wavefile.close();
	
	delete[] cue_chunk;
	return timeCue;
}

#include "main.h"

int main(int argc, char* argv[])
{
	//ifstream wavefile("die2.wav", ios::binary);

	//Get Audio Raw Data
	//unsigned long rawDataBufferSize = 0;
	//unsigned short* rawAudioData = getRawAudioData(wavefile, rawDataBufferSize);

	//Get Cue from Audio
	int nb_cue = 260;
	//float* time_cue = getMarker(wavefile, nb_cue);
	//wavefile.close();

	float cues[260] = 
	{
		48558,
		200119,
		320780,
		467927 ,
		525565 ,
		601831 ,
		698948 ,
		837266 ,
		919668 ,
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
		2166003,
		2441168,
		2626797,
		2868481,
		3040918,
		3184261,
		3377023,
		3550657,
		3704400,
		3878794,
		3987683,
		4193689,
		4332007,
		4499755,
		4751376,
		4926481,
		5055970,
		5151616,
		5367264,
		5626901,
		5841735,
		5974168,
		6084528,
		6184588,
		6269933,
		6342035,
		6434738,
		6524497,
		6630443,
		6734918,
		6811434,
		6930623,
		7040983,
		7136629,
		7282305,
		7422094,
		7517740,
		7611914,
		7762004,
		7907679,
		8032754,
		8143114,
		8254946,
		8360892,
		8411780,
		8459481,
		8540411,
		8612513,
		8709630,
		8789090,
		8902393,
		9029363,
		9167258,
		9248188,
		9326176,
		9377678,
		9423293,
		9467437,
		9507167,
		9557197,
		9601341,
		9651371,
		9699930,
		9744074,
		9789689,
		9836776,
		9877978,
		9933893,
		9983923,
		10028068,
		10075155,
		10262031,
		10357677,
		10482752,
		10578397,
		10659328,
		10787346,
		11022781,
		11255274,
		11446565,
		11593712,
		11701129,
		11839447,
		12254402,
		12547224,
		12638455,
		12750287,
		12853290,
		12950407,
		13084311,
		13194671,
		13294731,
		13402148,
		13519866,
		13633169,
		13768544,
		13899505,
		14023800,
		14112000,
		14256842,
		14355661,
		14442478,
		14508694,
		14602868,
		14677913,
		14770615,
		14928940,
		15091396,
		15188513,
		15357732,
		15450435,
		15591696,
		15693227,
		15772686,
		15887461,
		16005179,
		16111125,
		16214127,
		16318602,
		16401004,
		16526079,
		16648211,
		16771815,
		16960163,
		17276911,
		17460463,
		17662054,
		17873946,
		18034336,
		18185897,
		18302143,
		18500792,
		18653825,
		18870131,
		19064365,
		19177668,
		19308629,
		19433704,
		19583794,
		19747127,
		19856016,
		19904574,
		20110580,
		20269499,
		20460790,
		20582922,
		20727126,
		20908117,
		21069978,
		21299528,
		21451089,
		21532020,
		21633551,
		21705653,
		21779227,
		21846915,
		21946975,
		22057335,
		22163281,
		22267755,
		22360458,
		22510548,
		22642980,
		22741568,
		22862229,
		22959346,
		23044691,
		23138865,
		23246283,
		23324270,
		23434631,
		23570006,
		23664180,
		23740696,
		23864300,
		23939345,
		24071777,
		24137993,
		24242468,
		24324870,
		24407272,
		24483789,
		24635350,
		24745710,
		24825170,
		24894329,
		24951716,
		25116521,
		25231296,
		25347542,
		25434358,
		25510875,
		25600635,
		25705109,
		25968502,
		26081805,
		26177451,
		26271625,
		26396700,
		26496760,
		26582105,
		26673336,
		26767510,
		26858741,
		26913186,
		26995588,
		27089762,
		27182465,
		27276639,
		27364927,
		27425257,
		27498831,
		27618020,
		27713665,
		27810783,
		27900542,
		27962344,
		28022674,
		28174236,
		28269881,
		28346398,
		28415557,
		28525917,
		28595076,
		28667178,
		28786367,
		28898199,
		29048289,
		29151292,
		29251352,
		29336697,
		29410270,
		29558889,
		29714865,
		29836997,
		29987087,
		30103333,
		30219579

	};

	std::cout << "Array of cues loaded." << std::endl;
	for (int i = 0; i < 260; i++)
	{
		cues[i] /= 44100.0;
	}

	std::cout << "Array of cues devide by 44100." << std::endl;

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

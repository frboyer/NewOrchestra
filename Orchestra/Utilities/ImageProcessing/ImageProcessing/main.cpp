#include "main.h"

//-------------------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------------------
// ARGUMENTS	:	1- Folder Path
//					2- Images Name
//					3- Number of images to convert
// Optional		:
//					4- Output Image Name
//					5- Output Directory
//					6- Img Input Number Start
//-------------------------------------------------------------------------------
// OUTPUT FILES :	points.txt (Header : Number of Cue)
//					1- Point Number
//					2- Image Number
//					3- Top Left Point (X)
//					4- Top Left Point (Y)
//					5- Rectangle Size (X)
//					6- Rectangle Size (Y)
//-------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	//GET PROGRAM ARGUMENTS
	if(argc < 4 || argc > 7) 
	{
		cout << "Error : not enough arguments " << endl << endl;
		cout << "ARGUMENTS : 1- Folder Path" << endl <<
				"            2- Images Name" << endl <<
			    "            3- Number of images to convert" << endl;

		return 0;
	}

	wxString folderPath = wxString(argv[1]);
	folderPath = wxString(folderPath + wxString("/"));
	wxString imgName = wxString(argv[2]);
	int nbInputImages = atoi(argv[3]);
	
	wxString outputImgName;
	if(argc >= 5) outputImgName = wxString(argv[4]);
	else outputImgName = imgName;

	wxString outputPath;
	if(argc >= 6) outputPath = wxString(argv[5]);
	else outputPath = wxString("outputFiles/");
	
	int numImg = 1;
	if(argc >= 7) numImg = atoi(argv[6]);
	
	// Open output files.
	ofstream filePoint(wxString(outputPath + wxString("points.txt")).utf8_str());
    
	if(filePoint.fail()) 
	{
		cout << "Error : " << "can't open " << outputPath << "points.txt" << endl;
		return 0;
	}

	ofstream logFile(wxString(outputPath + wxString("log.txt")).utf8_str());
	if(logFile.fail()) 
	{
		cout << "Error : " << "can't open " << outputPath << "log.txt" << endl;
		filePoint.close();
		return 0;
	}

	//START READING IMAGES
	wxImage::AddHandler(new wxPNGHandler);
	filePoint << endl << endl;
	int totalPtCount = 0;
	int numImgSaved = 0;
	
	//SPLIT IMAGES LOOP
	while(nbInputImages--)
	{
		logFile << "-----------------------------------------------------------------" << endl;
		logFile << "Open : " << imgName + formatStringWithZero(numImg) << ".png" << endl;
		logFile << "-----------------------------------------------------------------" << endl;
		cout << "Open : " << imgName + formatStringWithZero(numImg) << ".png" << '\xd';

		wxImage img = openImage(getImageNumberPath(folderPath, imgName, numImg));
		if(!img.IsOk())
		{
			cout << "Error : " << "can't open " << getImageNumberPath(folderPath, imgName, numImg) << endl;
			filePoint.close();
			logFile.close();
			return 0;
		}

		//wxPoint ptTop, ptButtom; 
		//wxPoint lastPoint = getImageSeparationPoint(wxPoint(0,0), img);

		//SPLIT POINTS LOOP
		//while((ptButtom = getImageSeparationPoint(addOneToPoint(lastPoint), img)).x != -1)
		//{
			//logFile << "Split Point (" << numImgSaved << "), Top  : ("   
			//			W4 lastPoint.x << ", " W4 lastPoint.y << ")" << ", Buttom : (" 
			//			W4 ptButtom.x << ", " W4 ptButtom.y << ")" <<endl << endl;

			cout << "File : " << numImgSaved << '\xd'; 
			
			//ptTop = lastPoint;
			//wxImage subImg = splitImage(ptTop, ptButtom, img);

			int w = img.GetWidth();
			int h = img.GetHeight();

			//Draw white points on 4 corners
	/*		img.SetRGB(0, 0, 255, 255, 255);
			img.SetRGB(0, h - 1, 255, 255, 255);
			img.SetRGB(w - 1, 0, 255, 255, 255);
			img.SetRGB(w - 1, h - 1, 255, 255, 255);*/
	
			//LINE POINTS LOOP
			int nbPoint = 0; wxPoint lP, rP;
			wxPoint lastLinePt = getLineSeparationPoint(wxPoint(0,0), img);
			img.SetRGB(lastLinePt.x, lastLinePt.y, 0, 0, 0);
			
            while((rP = getLineSeparationPoint(addOneToPoint(lastLinePt), img)).x != -1)
			{
				img.SetRGB(rP.x, rP.y, 0, 0, 0);
				lP = lastLinePt;
			            
				logFile << "      Point (" << nbPoint << "), Left : (" \
				W4 lP.x << ", " W4 lP.y << ")" << ", Right  : (" W4 rP.x << ", " W4 rP.y <<")" << endl;
				
				//WRITE POINTS AND SIZES IN POINTS.TXT
				filePoint << formatStringWithZero(totalPtCount) << ' ' << formatStringWithZero(numImgSaved);
				
				if(lP.y < rP.y)
				{
					filePoint << ' ' W4 lP.x << ' ' << setw(3) << lP.y; //Point
					filePoint << ' ' W4 rP.x - lP.x << ' ' W4 rP.y - lP.y << endl; //Size
				}
				else
				{
					filePoint<< ' ' W4 lP.x << ' ' << setw(3) << rP.y; //Point
					filePoint << ' ' W4 rP.x - lP.x << ' ' W4 lP.y - rP.y << endl; //Size
				}

				++totalPtCount;
				++nbPoint;
				lastLinePt = rP;
			}

			logFile << endl <<"      Total : " << nbPoint << " mesures" << endl << endl;
			cout << nbPoint << " mesures" << '\xd';
			cout << "Export test" << numImgSaved << ".png" << '\xd';

			img.SaveFile(wxString::Format("outputFiles/test%s.png", formatStringWithZero(numImgSaved)), wxBITMAP_TYPE_PNG);
			//lastPoint = ptButtom;
			++numImgSaved;
		//}

		++numImg;
	}
	
	filePoint.seekp(0, ios::beg);
	filePoint << totalPtCount;
	
	filePoint.close();
	logFile.close();
	return 0;
}

wxString formatStringWithZero(int x)
{
	wxString s;
	if(x < 10) s = wxString::Format("00%d", x);
	else if(x < 100) s = wxString::Format("0%d", x);
	else s = wxString::Format("%d", x);

	return s;
}

wxString getImageNumberPath(const wxString folderName, const wxString fileName, int x)
{
	return folderName + fileName + formatStringWithZero(x) + wxString(".png");
}
wxImage openImage(wxString path)//, wxBitmap& bmp)
{
	wxBitmap bmp = wxBitmap(path, wxBITMAP_TYPE_PNG);
	return bmp.ConvertToImage();
}

wxPoint getImageSeparationPoint(wxPoint startPoint, wxImage& image)
{
	wxPoint pt;
	int h = image.GetHeight();
	int w = image.GetWidth();
	
	for(int y = startPoint.y; y < h; y++)
		for(int x = 0; x < w; x++)
			if(getGreenPixel(x, y, image)) 
			{
				pt = wxPoint(x, y);
				return pt;
			}
		
				
	return wxPoint(-1, -1);
}
wxPoint getLineSeparationPoint(wxPoint startPoint, wxImage& image)
{
	wxPoint pt;
	int h = image.GetHeight();
	int w = image.GetWidth();
	
	for(int x = startPoint.x; x < w; x++)
		for(int y = 0; y < h; y++)
			if(getRedPixel(x, y, image)) 
			{
				pt = wxPoint(x, y);
				return pt;
			}
		
	return wxPoint(-1, -1);
}
wxImage splitImage(wxPoint top, wxPoint buttom, wxImage& img)
{
	wxImage returnImg;
	if(top.x < buttom.x)
	{
		wxSize s(buttom.x - top.x, buttom.y - top.y);
		returnImg = img.GetSubImage(wxRect(top, s));
	}
	else
	{
		wxSize s(top.x - buttom.x, buttom.y - top.y);
		returnImg = img.GetSubImage(wxRect(wxPoint(buttom.x, top.y), s));
	}

	return returnImg;
}

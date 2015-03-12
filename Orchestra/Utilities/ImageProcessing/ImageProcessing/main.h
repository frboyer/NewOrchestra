#include <iostream>
#include <fstream>
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/image.h"
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

//---------------------------------------------
// SPLIT IMAGE   =  (0, 255, 0) 
// SPLIT MESURE  =  (255, 0, 0)  
//---------------------------------------------
struct t_point {short x, y;};
struct t_Size {short x, y;};
struct t_data
{
	unsigned int numSample;
	t_point point;
	t_Size size;
	short numImg;
};

//--------------------------------------------------------------------------------------
// TEXT DEFINE
//--------------------------------------------------------------------------------------
#define W4 <<setw(4)<<
		
/*#define PRINT_INFO_LINE(stream, numPt, l, r)                \             
		stream << "      Point (" << numPt << "), Left : (" \
		W4 l.x << ", " W4 l.y << ")" << ", Right  : (" W4 r.x << ", " W4 r.y <<")" << endl;*/
//--------------------------------------------------------------------------------------
// GLOBAL FUNCTION
//--------------------------------------------------------------------------------------
wxString formatStringWithZero(int x);
wxString getImageNumberPath(const wxString folderName, const wxString fileName, int x);
wxImage openImage(wxString path);

//If no more points in file return (-1, -1)
wxPoint getImageSeparationPoint(wxPoint startPoint, wxImage& image);
wxPoint getLineSeparationPoint(wxPoint startPoint, wxImage& image);
wxPoint findButtomLeftPoint(wxPoint top, wxPoint buttom);
wxImage splitImage(wxPoint top, wxPoint buttom, wxImage& img);

inline wxPoint addOneToPoint(wxPoint pt)
{
	return wxPoint(pt.x + 1, pt.y + 1);
}
inline bool getGreenPixel(int x, int y, wxImage& img)
{
	return img.GetRed(x, y) == 0 && img.GetBlue(x, y) == 0 && img.GetGreen(x, y) == 255;
}

inline bool getRedPixel(int x, int y, wxImage& img)
{
	return img.GetRed(x, y) == 255 && img.GetBlue(x, y) == 0 && img.GetGreen(x, y) == 0;
}




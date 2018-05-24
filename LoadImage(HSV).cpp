// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"
#include <fstream>
#include <string>
using namespace std;
#define SIZE 4 
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

bool IsEqual(double a, double b)
{
	return fabs(a - b) < 0.001;
}

//RBG×ªHSV
void RGBtoHSV(BYTE byteR, BYTE byteG, BYTE byteB, int &h, double &s, double &v)
{
	double h_double;
	double r, g, b;
	double min, max;
	double delta;

	r = byteR / 255.0;
	g = byteG / 255.0;
	b = byteB / 255.0;

	if (r > g)
	{
		max = MAX(r, b);
		min = MIN(g, b);
	}
	else
	{
		max = MAX(g, b);
		min = MIN(r, b);
	}

	v = max;

	delta = max - min;
	if (IsEqual(max, 0))
		s = 0.0;
	else
		s = delta / max;

	if (max == min)
		h_double = 0.0;
	else
	{
		if (IsEqual(r, max) && g >= b)
			h_double = 60 * (g - b) / delta + 0;
		else if (IsEqual(r, max) && g < b)
			h_double = 60 * (g - b) / delta + 360;
		else if (IsEqual(g, max))
			h_double = 60 * (b - r) / delta + 120;
		else if (IsEqual(b, max))
			h_double = 60 * (r - g) / delta + 240;
	}
	h = (int)(h_double + 0.5);
	h = (h > 359) ? (h - 360) : h;
	h = (h < 0) ? (h + 360) : h;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream out("HSV_histogram.txt", ios::trunc);
	CString image_name[3] = { "AR0001_1m.jpg", "n01613177_60.JPEG", "n01613177_104.JPEG" };


	for (int image_i = 0; image_i < 3; image_i++)
	{
		CImage image;
		double HSV_Hist[8][3][3] = { 0 };
		/*
		h = 0£¬H¡Ê[316, 20]      s = 0£¬S¡Ê[0, 0.2] 
		h = 1£¬H¡Ê[21, 40]       s = 1£¬S¡Ê[0.2, 0.7]
		h = 2£¬H¡Ê[41, 75]       s = 2£¬S¡Ê[0.7, 1] 
		h = 3£¬H¡Ê[76, 155]      
		h = 4£¬H¡Ê[156, 190]     v = 0£¬V¡Ê[0, 0.2] 
		h = 5£¬H¡Ê[191, 270]     v = 1£¬V¡Ê[0.2, 0.7]
		h = 6£¬H¡Ê[271, 295]     v = 2£¬V¡Ê[0.7, 1] 
		h = 7£¬H¡Ê[296, 315]
		*/

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;
		int h;
		double s, v;

		image.Load(image_name[image_i]);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();

		for (int iRow = 0; iRow < iWidth; iRow++)
			for (int iCol = 0; iCol < iHeight; iCol++)
			{
				COLORREF colorref = image.GetPixel(iRow, iCol);

				if (colorref == CLR_INVALID)
				{
					printf("Out of range!");
					continue;
				}

				byteR = GetRValue(colorref);
				byteG = GetGValue(colorref);
				byteB = GetBValue(colorref);
				
				RGBtoHSV(byteR, byteG, byteB, h, s, v);
				int H, S, V;

				if ((h >= 316 && h < 360) || (h >= 0 && h <= 20))
					H = 0;
				else if (h >= 21 && h <= 40)
					H = 1;
				else if (h >= 41 && h <= 75)
					H = 2;
				else if (h >= 76 && h <= 155)
					H = 3;
				else if (h >= 156 && h <= 190)
					H = 4;
				else if (h >= 191 && h <= 270)
					H = 5;
				else if (h >= 271 && h <= 295)
					H = 6;
				else if (h >= 296 && h <= 315)
					H = 7;

				if (s >= 0 && s <= 0.2)
					S = 0;
				else if (s > 0.2 && s < 0.7)
					S = 1;
				else if (s >= 0.7 && s <= 1)
					S = 2;

				if (v >= 0 && v <= 0.2)
					V = 0;
				else if (v > 0.2 && v < 0.7)
					V = 1;
				else if (v >= 0.7 && v <= 1)
					V = 2;

				HSV_Hist[H][S][V]++;
				//printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);		

				//RGB_Hist[byteR / (256 / SIZE)][byteG / (256 / SIZE)][byteB / (256 / SIZE)]++;
			}

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
				{
					out << HSV_Hist[i][j][k] << " ";
				}
			//out << endl;
		}
		out << endl;
		cout << "finish." << endl;
		image.GetBits();

		image.Destroy();
	}
	getchar();

	return 0;
}

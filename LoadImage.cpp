// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"
#include <fstream>
#include <string>
using namespace std;
#define SIZE 4 

int _tmain(int argc, _TCHAR* argv[])
{
	ofstream out("color_histogram.txt", ios::trunc);
	CString image_name[3] = { "AR0001_1m.jpg", "n01613177_60.JPEG", "n01613177_104.JPEG" };
	//string image_name[3] = {"AR0001_1m.jpg", "n01613177_60.JPEG", "n01613177_104.JPEG"};


	for (int image_i = 0; image_i < 3; image_i++)
	{
		CImage image;
		int Color_Hist[SIZE][SIZE][SIZE] = { 0 };

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;

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

				//printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);		

				Color_Hist[byteR / (256 / SIZE)][byteG / (256 / SIZE)][byteB / (256 / SIZE)]++;
			}

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				for (int k = 0; k < SIZE; k++)
				{
					out << Color_Hist[i][j][k] << " ";
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

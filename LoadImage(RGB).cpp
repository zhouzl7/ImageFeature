// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
#define SIZE 2 


void readImage(string filename, vector<string> &Array)
{
	ifstream inFile(filename);
	if (!inFile.is_open())
	{
		fprintf(stderr, "%s not exist\n", filename.c_str());
		return;
	}
	string str;
	while (getline(inFile, str))
	{
		Array.push_back(str);
	}
	inFile.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	string image_Filename = "imagelist.txt";
	vector<string> imageArray;  //ËùÓÐµÄimagename

	ofstream out("RGB_histogram.txt", ios::trunc);
	readImage(image_Filename, imageArray);
	CString image_name[6000];
	for (int i = 0; i < imageArray.size(); i++)
	{
		string a = string("image\\") + imageArray[i];
		image_name[i] = a.c_str();
	}
	out << "tech nmos<< polysilicon >>rect";


	for (int image_i = 0; image_i < imageArray.size(); image_i++)
	{
		CImage image;
		int RGB_Hist[SIZE][SIZE][SIZE] = { 0 };

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
				double size = 256 * 1.0 / SIZE + 0.5;
				RGB_Hist[byteR / (int)size][byteG / (int)size][byteB / (int)size]++;
			}

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
				for (int k = 0; k < SIZE; k++)
				{
					out << " " << RGB_Hist[i][j][k];
				}
		}
		if (image_i == imageArray.size() - 1)
			out << "<< end >>";
		else
			out << "rect";
		cout << image_i << endl;
		image.GetBits();

		image.Destroy();
	}
	getchar();

	return 0;
}

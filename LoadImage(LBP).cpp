// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
#define BLOCK 4  //分块BLOCK*BLOCK
#define SIZE 4 //LBP值划分区间个数


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


//calculate 灰度值
double RGBtoGray(BYTE R, BYTE G, BYTE B)
{
	double Gray;
	Gray = R * 0.3 + G * 0.59 + B * 0.11;
	return Gray;
}

//calculate LBP值
int LBP_value(double **GrayArray, int i, int j)
{
	int neighbor[8] = { 0 };
	neighbor[7] = GrayArray[i - 1][j - 1];
	neighbor[6] = GrayArray[i - 1][j];
	neighbor[5] = GrayArray[i - 1][j + 1];
	neighbor[4] = GrayArray[i][j + 1];
	neighbor[3] = GrayArray[i + 1][j + 1];
	neighbor[2] = GrayArray[i + 1][j];
	neighbor[1] = GrayArray[i + 1][j - 1];
	neighbor[0] = GrayArray[i][j - 1];
	int temp = 0;
	for (int k = 0; k < 8; k++)
	{
		temp += (neighbor[k] >= GrayArray[i][j]) << k;
	}
	return temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string image_Filename = "imagelist.txt";
	vector<string> imageArray;  //所有的imagename

	ofstream out("LBP_feature.txt", ios::trunc);
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

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;

		image.Load(image_name[image_i]);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();
		double **GrayArray = new double*[iHeight];
		int **LBP = new int*[iHeight];
		int LBP_Hist[BLOCK][BLOCK][SIZE] = { 0 };//LBP直方图
		/*
		将图片划分为BLOCK*BLOCK个方块，统计每个方块的LBP直方图，LBP∈[0, 255]
		所以，特征向量共有BLOCK*BLOCK*256维
		*/
		for (int i = 0; i < iHeight; i++)
		{
			GrayArray[i] = new double[iWidth];
			LBP[i] = new int[iWidth];
		}

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
				GrayArray[iCol][iRow] = RGBtoGray(byteR, byteG, byteB);
				//printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);		
			}

		for (int i = 1; i < iHeight - 1; i++)
			for (int j = 1; j < iWidth - 1; j++)
			{
				LBP[i][j] = LBP_value(GrayArray, i, j);
				double size1 = iHeight * 1.0 / BLOCK + 0.5;
				double size2 = iWidth * 1.0 / BLOCK + 0.5;
				double size3 = 256 * 1.0 / SIZE + 0.5;
				LBP_Hist[i / (int)size1][j / (int)size2][LBP[i][j] / (int)size3]++;
				/*if (i < iHeight / 2)
				{
					if (j < iWidth / 2)
						LBP_Hist[0][LBP[i][j]]++;
					else
						LBP_Hist[1][LBP[i][j]]++;
				}
				else
				{
					if (j < iWidth / 2)
						LBP_Hist[2][LBP[i][j]]++;
					else
						LBP_Hist[3][LBP[i][j]]++;
				}*/
			}


		for (int i = 0; i < BLOCK; i++)
		{
			for (int k = 0; k < BLOCK; k++)
			{
				for (int j = 0; j < SIZE; j++)
					out << " " << LBP_Hist[i][k][j];
			}
		}
		if (image_i == imageArray.size() - 1)
			out << "<< end >>";
		else
			out << "rect";
		cout << image_i << endl;

		image.GetBits();
		image.Destroy();
		for (int i = 0; i < iHeight; i++)
		{
			delete[]LBP[i];
			delete[]GrayArray[i];
		}
		delete[]LBP;
		delete[]GrayArray;
	}
	getchar();

	return 0;
}

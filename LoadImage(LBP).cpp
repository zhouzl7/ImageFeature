// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"
#include <fstream>
#include <string>
using namespace std;

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
	ofstream out("LBP_feature.txt", ios::trunc);
	CString image_name[3] = { "AR0001_1m.jpg", "n01613177_60.JPEG", "n01613177_104.JPEG" };


	for (int image_i = 0; image_i < 3; image_i++)
	{
		CImage image;

		int iHeight, iWidth;
		BYTE byteR, byteG, byteB;

		image.Load(image_name[image_i]);

		iHeight = image.GetHeight();
		iWidth = image.GetWidth();
		double **GrayArray = new double*[iHeight];
		int **LBP = new int*[iHeight];
		int LBP_Hist[4][256] = { 0 };//LBP直方图
		/*
		将图片划分为2*2的4个方块，统计每个方块的LBP直方图，LBP∈[0, 255]
		所以，特征向量共有4*256维
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
				if (i < iHeight / 2)
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
				}
			}


		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 256; j++)
				out << LBP_Hist[i][j] << " ";
			out << endl;
		}
		out << endl << endl;
		cout << "finish." << endl;

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

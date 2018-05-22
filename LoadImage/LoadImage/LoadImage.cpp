// LoadImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlimage.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CImage image;

	int iHeight, iWidth;
	BYTE byteR, byteG, byteB;

	image.Load( "AR0001_1m.jpg" );

	iHeight = image.GetHeight();
	iWidth = image.GetWidth();

	for ( int iRow=0; iRow<iWidth; iRow++)
		for ( int iCol=0; iCol<iHeight; iCol++ )
		{
			COLORREF colorref = image.GetPixel( iRow, iCol );

			if ( colorref==CLR_INVALID )
			{
				printf("Out of range!");
				continue;
			}

			byteR =  GetRValue( colorref );
			byteG =  GetGValue( colorref );
			byteB =  GetBValue( colorref );

			printf("%Pixel at (%d,%d) is: R=0x%x,G=0x%x,B=0x%x\n",iRow, iCol, byteR, byteG, byteB);			
		}

	image.GetBits();

	image.Destroy();

	getchar();

	return 0;
}

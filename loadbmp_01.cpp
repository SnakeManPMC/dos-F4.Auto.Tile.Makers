// *************************************************************
//                       BMP LOAD EXAMPLE
//         Written by Juan Soulie <jsoulie@cplusplus.com>
// *************************************************************
//                  11/23/2002 Tomas GPozuelo
//                    <Com_GP@hotmail.com>
// *************************************************************

#include <windows.h>
#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>

// **********
// class CRaster
//   - Generic class for BMP raster images.
class CRaster {
	public:
		int Width,Height;		// Dimensions
		int BPP;				// Bits Per Pixel.
		char * Raster;			// Bits of the Image.
		RGBQUAD * Palette;		// RGB Palette for the image.
		int BytesPerRow;		// Row Width (in bytes).
		BITMAPINFO * pbmi;		// BITMAPINFO structure

		// Member functions (defined later):
		void LoadBMP (char * szFile);
//		int GDIPaint (HDC hdc,int x,int y);
};


// **********
// CRaster::LoadBMPFile (FileName);
//   - loads a BMP file into a CRaster object
//   * supports non-RLE-compressed files of 1, 2, 4, 8 & 24 bits-per-pixel
void CRaster::LoadBMP (char * szFile)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// Open file.
	ifstream bmpfile (szFile , ios::in | ios::binary);
	if (! bmpfile.is_open())
        cout << "ERROR OPENING FILE" << endl;		// Error opening file

	// Load bitmap fileheader & infoheader
	bmpfile.read ((char*)&bmfh,sizeof (BITMAPFILEHEADER));
	bmpfile.read ((char*)&bmih,sizeof (BITMAPINFOHEADER));

	// Check filetype signature
	if (bmfh.bfType!='MB')
        cout << "ERROR FILE IS NOT BMP" << endl;		// File is not BMP

	// Assign some short variables:
	BPP=bmih.biBitCount;      //bits per pixel
	Width=bmih.biWidth;
	Height= (bmih.biHeight>0) ? bmih.biHeight : -bmih.biHeight; // absoulte value
	BytesPerRow = Width * BPP / 8;
	BytesPerRow += (4-BytesPerRow%4) % 4;	// int alignment

	// If BPP aren't 24, load Palette:
	if (BPP==24) pbmi=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
	else
	{
		pbmi=(BITMAPINFO*) new char[sizeof(BITMAPINFOHEADER)+(1<<BPP)*sizeof(RGBQUAD)];
		Palette=(RGBQUAD*)((char*)pbmi+sizeof(BITMAPINFOHEADER));
		bmpfile.read ((char*)Palette,sizeof (RGBQUAD) * (1<<BPP));
	}
	pbmi->bmiHeader=bmih;

	// Load Raster
	bmpfile.seekg (bmfh.bfOffBits,ios::beg);

	Raster= new char[BytesPerRow*Height];

	// (if height is positive the bmp is bottom-up, read it reversed)
	if (bmih.biHeight>0)
		for (int n=Height-1;n>=0;n--)
			bmpfile.read (Raster+BytesPerRow*n,BytesPerRow);
	else
		bmpfile.read (Raster,BytesPerRow*Height);

	// so, we always have a up-bottom raster (that is negative height for windows):
	pbmi->bmiHeader.biHeight=-Height;

	bmpfile.close();
}

int main()
{
	BITMAPINFO *bmi_aux;
    char * Raster_aux;
    static CRaster bmp;
//	HDC hdc;
	PAINTSTRUCT ps;

			bmp.LoadBMP ("example.bmp");
//            bmi_aux = bmp.pbmi;
            Raster_aux = bmp.Raster;
//            cout << bmi_aux << endl;
            cout << Raster_aux << endl;
            cout << *Raster_aux << endl;
            system("PAUSE");

    return 0;
}

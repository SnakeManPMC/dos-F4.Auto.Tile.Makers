// *************************************************************
//                       BMP LOAD EXAMPLE
//         Written by Juan Soulie <jsoulie@cplusplus.com>
// *************************************************************

#include <windows.h>
#include <fstream.h>

char szAppName [] = "BMPLoad";

LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);

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
		int LoadBMP (char * szFile);
		int GDIPaint (HDC hdc,int x,int y);
};

// **********
// Windows Main Function. 
//   - Here starts our demo program
int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow )
{
	HWND hwnd;
	MSG msg;

	WNDCLASS wc;
	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass (&wc);

	hwnd = CreateWindow (szAppName,"BMP Load",WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		0,0,hInstance,0);

	ShowWindow (hwnd,nCmdShow);
	UpdateWindow (hwnd);

	while (GetMessage(&msg,0,0,0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return msg.wParam;
}

// **********
// Main Window Procedure.
//   - Processes Window Messages
LRESULT CALLBACK WindowProc
	(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CRaster bmp;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
		case WM_CREATE:
			bmp.LoadBMP ("example.bmp");
			return 0;
		case WM_PAINT:
			hdc=BeginPaint (hwnd,&ps);
			bmp.GDIPaint (hdc,10,10);
			EndPaint (hwnd,&ps);
			return 0;
		case WM_DESTROY:
			PostQuitMessage (0);
			return 0;
	}
	return DefWindowProc (hwnd,message,wParam,lParam);
}

// **********
// CRaster::LoadBMPFile (FileName);
//   - loads a BMP file into a CRaster object
//   * supports non-RLE-compressed files of 1, 2, 4, 8 & 24 bits-per-pixel
int CRaster::LoadBMP (char * szFile)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// Open file.
	ifstream bmpfile (szFile , ios::in | ios::binary);
	if (! bmpfile.is_open()) return 1;		// Error opening file

	// Load bitmap fileheader & infoheader
	bmpfile.read ((char*)&bmfh,sizeof (BITMAPFILEHEADER));
	bmpfile.read ((char*)&bmih,sizeof (BITMAPINFOHEADER));

	// Check filetype signature
	if (bmfh.bfType!='MB') return 2;		// File is not BMP

	// Assign some short variables:
	BPP=bmih.biBitCount;
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

	return 0;
}

// **********
// CRaster::GDIPaint (hdc,x,y);
// * Paints Raster to a Windows DC.
int CRaster::GDIPaint (HDC hdc,int x=0,int y=0)
{
	// Paint the image to the device.
	return SetDIBitsToDevice (hdc,x,y,Width,Height,0,0,
		0,Height,(LPVOID)Raster,pbmi,0);
}

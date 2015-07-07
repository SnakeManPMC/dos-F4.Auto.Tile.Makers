   *************************************************************
                         BMP LOAD EXAMPLE
           Written by Juan Soulie <jsoulie@cplusplus.com>
   *************************************************************

This package includes the following files:

 LOADBMP.CPP    Simple Windows (Win32) application source to showing
                how to load a BMP into memory and to paint it.
 LOADBMP.EXE    Compiled Executable.
 EXAMPLE.BMP    Example BMP file (100x100x256).
 README.TXT     This.

	This example loads a BMP file into an object of class CRaster
that includes members to store Palette, Raster bits, and Bitmap
information.

	CRaster includes a member function (GDIPaint) to paint the
raster directly to a Windows' device (like the screen).

	This program shall be compiled as a Win32 application.

---------------------------------------------------------------------
                      July 2000, Juan Soulie <jsoulie@cplusplus.com>
---------------------------------------------------------------------

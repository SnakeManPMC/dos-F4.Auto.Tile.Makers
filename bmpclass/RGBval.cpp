/* 
   Name: testmain.cpp, MAIN FUNCTION FOR THE TRANSITION PROGRAM
   Author: Tomas Garcia-Pozuelo Barrios
   Description: AUTOMATIC TRANSITION TILE MAKER FOR FALCON 4.0
   Date: Started 03/12/2002
   Copyright: 
*/

//INCLUDE´s
#include "bmpmanclass.h"
#include <math.h>


int String2Int( string s )
{
  int i = 0, x = 0;

  while ( i < s.length() )
  {
   x = x * 10 + int( s[i] - '0' );
   i++;
  }
  return x;
}



int main()
{
 bmpman BMP;
 string string_1;
 RGBQUAD rgb;
 int acum = 0;

 int Red = 0, Blue = 0, Green = 0;
 int nfiles;
 int totalleft;
 ofstream pr;
 ifstream ifs;

 pr.open("theater.pal");
 if(!pr)
     cout << "ERROR GUARDANDO EN EL FICHERO" << endl;
 else{

  pr << "JASC-PAL\n0100\n256\n255 255 255\n8 8 8\n99 99 99\n";

  ifs.open("lstiles.txt");
  if(!ifs)
      cout << "ERROR OPENING THE SCRIPT FILE" << endl;
  else{

       getline( ifs, string_1);
       nfiles = String2Int( string_1 );
       totalleft = 253 - nfiles;
       cout << "NUMBER OF FILES = " << nfiles << endl;
       while( nfiles > 0 ){

           Red = 0;
           Blue = 0;
           Green = 0;
           acum = 0;
           getline( ifs, string_1 );
           cout << "NAME OF THE FIRST 24bpp BITMAP (Name1.bmp) --> " << string_1 << endl;
           BMP.LoadBMP(string_1.c_str());

      for( int i = 0; i < BMP.GetWidth(); i++ ){
           for( int j = 0; j < BMP.GetHeigth(); j++ ){
                rgb = BMP.GetClr( i, j );
                Red = Red + int(rgb.rgbRed);
                Green = Green + int(rgb.rgbGreen);
                Blue = Blue + int(rgb.rgbBlue);

                acum++;
           }
      }

      Red = Red / acum;
      Green = Green / acum;
      Blue = Blue / acum;

      pr << Red << " " << Green << " " << Blue << endl;


           nfiles--;
       }

  ifs.close();
  }
 for ( int i = 0; i < totalleft; i++ )
      pr << "0 0 0" << endl;
 pr.close();
 }


//*************



   system("PAUSE");
   return 0;
}


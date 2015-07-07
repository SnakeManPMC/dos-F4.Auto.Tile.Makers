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

//PROTOTIPES

int main()
{
   //VARIABLES
   bmpman BMP1;
   string string_1;
   int width_tile;

   cout << "NAME OF TILE" << endl;
   cin >> string_1;

   BMP1.LoadBMP("164.bmp");
   width_tile = BMP1.GetWidth();



   cout << width_tile << endl;
   system("PAUSE");
   cout << endl << "PROGRESS ";
   int x, y;
   for( x = 0; x < width_tile; x++ )
      for( y = 0; y < width_tile; y++ ){
          BMP1.EnterRGBdata( x, y, BMP1.LoadPixelBMP( string_1, x, y ) );
          cout << x << " " << y << endl;


          }
   cout << endl;
//   BMP1.EnterRGBdata( x, y, BMP1.LoadPixelBMP( string_1, x+1, y+1 ) );


   BMP1.SaveBMP("Htest.bmp");


   system("PAUSE");
   return 0;
}


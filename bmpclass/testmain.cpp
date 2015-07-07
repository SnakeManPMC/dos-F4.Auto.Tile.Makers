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
void Htile1( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile2( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile3( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile4( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile5( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile7( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void Htile8( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );

void HtileA( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void HtileB( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void HtileC( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void HtileD( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );
void HtileE( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact );

int MakeTransition( string string_1, string string_2, int cin_aux, int factor );
int String2Int( string s );


int main()
{

     int x, y, h, w;
     int nfiles, smooth;//smooth no se usa todavia
     int factor, factor1;

     string s_aux;
     string file1, file2;

     ifstream f;


     cout << "***********************************************************\n";
     cout << "*  AUTOTRANSITION MAKER WITH 128x128 AND 256x256 SUPPORT  *\n";
     cout << "***********************************************************" << endl << endl << "By Com_GP (Com_GP@yahoo.com) 06/12/2002" << endl << endl;
     cout << "SCRIPT DRIVEN, the script should be in the same folder and its\ncalled bmplist.txt\n";

     f.open("bmplist.txt");
     if(!f){
           cout << "ERROR OPENING THE SCRIPT FILE" << endl;
           system("PAUSE");
     }
     else{

      getline( f, s_aux);
      nfiles = String2Int( s_aux );
      cout << endl << "NUMBER OF BMPs IN THE SCRIPT -> " << nfiles << endl;
      getline( f, s_aux);
      factor = String2Int( s_aux );
      cout << "TRANSITION AREA 1, 2, 3 ------> " << factor << endl;
      getline( f, s_aux);
      factor1 = String2Int( s_aux );
      cout << "SMOOTH FACTOR ----------------> " << factor1 << endl << endl;

      //TEMP supuestamente hay que leerlo del fichero de texto
//      cin >> factor1;

      system("PAUSE");

      for( x = 1; x <= nfiles; x++ ){
          f.seekg(0);
          getline( f, s_aux);
          getline( f, s_aux);
          getline( f, s_aux);
          for( w = 0; w < x; w++ ){
              getline( f, file1);
          }
          for( y = x; y < nfiles; y++ ){
              f.seekg(0);
              getline( f, s_aux);
              getline( f, s_aux);
              getline( f, s_aux);
              getline( f, s_aux);
              for( h = 0; h < y; h++ )
                   getline( f, file2);
                   if( file1 != file2 )
                       if( MakeTransition( file1, file2, factor, factor1 ) == 0 )
                           return 0;
//                   cout << "File1 = " << file1 << " File2 = " << file2 << endl;
          }
      }
     }
   return 0;
}

void Htile1( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )//, int smooth )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(1) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < trans_area; x++){
             for( y = 0; y < trans_area; y++){
                  dist = int(sqrt( (x*x) + (y*y) ));
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2 = (changpixels2 + 1);
   perctg1 = ( perctg1 - (20/fact) );

   cout << ".";
   }
   cout << "Done\n";
}

//Htile2
void Htile2( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(2) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < trans_area; x++){
             for( y = (BMP1.GetWidth()-trans_area); y < BMP1.GetWidth(); y++){
                  dist = int(sqrt( (x*x) + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) ));
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//Htile4
void Htile4( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(4) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = (BMP1.GetWidth()-trans_area); x < BMP1.GetWidth(); x++){
             for( y = 0; y < trans_area; y++){
                  dist = int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + (y*y) ));
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//Htile7
void Htile7( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact  )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   int trans_area_n = BMP1.GetWidth() - trans_area;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 1;//2;
   cout << "Htile(7) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < (BMP2.GetWidth()); x++){
             for( y = 0; y < (BMP2.GetWidth()); y++){
                  dist = (int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) )) );
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= (dist - trans_area_n )) && ((dist - trans_area_n ) < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char((((rgb_aux1.rgbRed)*(100-perctg1))/100)+(((rgb_aux2.rgbRed)*perctg1)/100));
                           rgb_auxRES.rgbBlue = char((((rgb_aux1.rgbBlue)*(100-perctg1))/100)+(((rgb_aux2.rgbBlue)*perctg1)/100));
                           rgb_auxRES.rgbGreen = char((((rgb_aux1.rgbGreen)*(100-perctg1))/100)+(((rgb_aux2.rgbGreen)*perctg1)/100));                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( fact == 4 ){
                           if( dist >= (BMP2.GetWidth() - ((18*trans_area)/96)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
                       else{
                           if( dist >= (BMP2.GetWidth() - (trans_area/8)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 >= (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//Htile8
void Htile8( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact  )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(8) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = (BMP1.GetWidth()-trans_area); x < BMP1.GetWidth(); x++){
             for( y = (BMP1.GetWidth()-trans_area); y < BMP1.GetWidth(); y++){
                  dist = int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) ));
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//HtileB
void HtileB( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   int trans_area_n = BMP1.GetWidth() - trans_area;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 1;//2;
   cout << "Htile(b) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < (BMP2.GetWidth()); x++){
             for( y = 0; y < (BMP2.GetWidth()); y++){
                  dist = (int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + y*y )) );
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= (dist- trans_area_n)) && ((dist- trans_area_n) < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char((((rgb_aux1.rgbRed)*(100-perctg1))/100)+(((rgb_aux2.rgbRed)*perctg1)/100));
                           rgb_auxRES.rgbBlue = char((((rgb_aux1.rgbBlue)*(100-perctg1))/100)+(((rgb_aux2.rgbBlue)*perctg1)/100));
                           rgb_auxRES.rgbGreen = char((((rgb_aux1.rgbGreen)*(100-perctg1))/100)+(((rgb_aux2.rgbGreen)*perctg1)/100));                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( fact == 4 ){
                           if( dist >= (BMP2.GetWidth() - ((18*trans_area)/96)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
                       else{
                           if( dist >= (BMP2.GetWidth() - (trans_area/8)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 >= (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//HtileD
void HtileD( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   int trans_area_n = BMP1.GetWidth() - trans_area;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 1;//2;
   cout << "Htile(d) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < (BMP2.GetWidth()); x++){
             for( y = 0; y < (BMP2.GetWidth()); y++){
                  dist = (int(sqrt( x*x + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) )) );
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= (dist- trans_area_n)) && ((dist- trans_area_n) < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char((((rgb_aux1.rgbRed)*(100-perctg1))/100)+(((rgb_aux2.rgbRed)*perctg1)/100));
                           rgb_auxRES.rgbBlue = char((((rgb_aux1.rgbBlue)*(100-perctg1))/100)+(((rgb_aux2.rgbBlue)*perctg1)/100));
                           rgb_auxRES.rgbGreen = char((((rgb_aux1.rgbGreen)*(100-perctg1))/100)+(((rgb_aux2.rgbGreen)*perctg1)/100));                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( fact == 4 ){
                           if( dist >= (BMP2.GetWidth() - ((18*trans_area)/96)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
                       else{
                           if( dist >= (BMP2.GetWidth() - (trans_area/8)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 >= (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//HtileE
void HtileE( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   int trans_area_n = BMP1.GetWidth() - trans_area;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 1;//2;
   cout << "Htile(e) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < (BMP2.GetWidth()); x++){
             for( y = 0; y < (BMP2.GetWidth()); y++){
                  dist = (int(sqrt( x*x + y*y )) );
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= (dist- trans_area_n)) && ((dist- trans_area_n) < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char((((rgb_aux1.rgbRed)*(100-perctg1))/100)+(((rgb_aux2.rgbRed)*perctg1)/100));
                           rgb_auxRES.rgbBlue = char((((rgb_aux1.rgbBlue)*(100-perctg1))/100)+(((rgb_aux2.rgbBlue)*perctg1)/100));
                           rgb_auxRES.rgbGreen = char((((rgb_aux1.rgbGreen)*(100-perctg1))/100)+(((rgb_aux2.rgbGreen)*perctg1)/100));                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( fact == 4 ){
                           if( dist >= (BMP2.GetWidth() - ((18*trans_area)/96)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
                       else{
                           if( dist >= (BMP2.GetWidth() - (trans_area/8)) )
                               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 >= (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );
   cout << ".";
   }
   cout << "Done\n";
}

//Htile3
void Htile3( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(3) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < trans_area; x++){
             for( y = 0; y < BMP2.GetWidth(); y++){
                  dist = x;
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );

   cout << ".";
   }
   cout << "Done\n";
}

//HtileC
void HtileC( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(c) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = BMP1.GetWidth() - trans_area; x < BMP1.GetWidth(); x++){
             for( y = 0; y < BMP2.GetWidth(); y++){
                  dist = BMP1.GetWidth() - x;
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );

   cout << ".";
   }
   cout << "Done\n";
}

//HtileA
void HtileA( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(a) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < BMP1.GetWidth(); x++){
             for( y = BMP1.GetWidth() - trans_area; y < BMP1.GetWidth(); y++){
                  dist = BMP1.GetWidth() - y;
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );

   cout << ".";
   }
   cout << "Done\n";
}

//Htile5
void Htile5( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int trans_area, int fact )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   perctg1 = 100;
   changpixels1 = 0;
   changpixels2 = 2;
   cout << "Htile(5) ";
   while( perctg1 >= 0 ){ //Condition
        for( x = 0; x < BMP1.GetWidth(); x++){
             for( y = 0; y < trans_area; y++){
                  dist = y;
                       if( ( ((changpixels1*trans_area)/(fact*8)) <= dist) && (dist < ((changpixels2*trans_area)/(fact*8))) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
             }
        }

   changpixels1 = changpixels2;
   if( changpixels2 < (fact*4) )
       changpixels2 = (changpixels2 + 2);
   else
       changpixels2++;
   perctg1 = ( perctg1 - (20/fact) );

   cout << ".";
   }
   cout << "Done\n";
}

int MakeTransition( string string_1, string string_2, int cin_aux, int factor )
{

   //VARIABLES
   bmpman BMP1, BMP2;
   int trans_area;
   string savename;

   cout << "NAME OF THE FIRST 24bpp BITMAP (Name1.bmp) -->";
   cout << string_1 << endl;
//   cin >> string_1;

   cout << "NAME OF THE SECOND 24bpp BITMAP (Name2.bmp) -->";
   cout << string_2 << endl;
//   cin >> string_2;

   BMP1.LoadBMP(string_1.c_str());
   BMP2.LoadBMP(string_2.c_str());

   //Same Width and Height
   if( BMP1.GetWidth() != BMP1.GetHeigth() ){
       cout << "ERROR, THE FIRST BITMAP HEIGHT IS NOT THE SAME AS THE WIDTH" << endl;
       system("PAUSE");
       return 0;
   }

   //Same Width and Height
   if( BMP2.GetWidth() != BMP2.GetHeigth() ){
       cout << "ERROR, THE SECOND BITMAP HEIGHT IS NOT THE SAME AS THE WIDTH" << endl;
       system("PAUSE");
       return 0;
   }

   //The bmp1 load right
   if( BMP1.GetWidth() == 0 ){
       cout << "ERROR, THE FIRST BMP IS NOT RIGHT LOADED (probably wrong typing)" << endl;
       system("PAUSE");
       return 0;
   }

   //The bmp2 load right
   if( BMP2.GetWidth() == 0 ){
       cout << "ERROR, THE SECOND BMP IS NOT RIGHT LOADED (probably wrong typing)" << endl;
       system("PAUSE");
       return 0;
   }

   //Same Width in the two tiles
   if( BMP1.GetWidth() != BMP2.GetWidth() ){
       cout << "ERROR, THE TILES DONT HAVE THE SAME WIDTH OR HEIGHT" << endl;
       system("PAUSE");
       return 0;
   }

   cout << "The Transition IS SET to 1/4 of the total Width/Height of the tile\nTransition area FACTOR(1 is 32 pixels in a 128x128 tile)(1-3) -> ";
   cout << cin_aux << endl;
//   cin >> cin_aux;
   trans_area = (cin_aux*BMP1.GetWidth()/4);//must be an int


   cout << endl << "Transition Square " << trans_area << " Pixels" << endl;
//   system("PAUSE");


   bmpman BMP24bpp1(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp2(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp3(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp4(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp5(BMP1.GetWidth(), BMP1.GetWidth(), 24);

   bmpman BMP24bpp7(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp8(BMP1.GetWidth(), BMP1.GetWidth(), 24);

   bmpman BMP24bppA(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppB(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppC(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppD(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppE(BMP1.GetWidth(), BMP1.GetWidth(), 24);


   //cambiar tamaños para contemplar 256x256
//   bmpman BMP24bppBIG;//(5*(BMP1.GetHeigth()+2)), (4*(BMP1.GetHeigth()+2)), 24 );
//   BMP24bppBIG.LoadBMP("bmpbase.dat");

   bmpman BMP24bppBIG(  4*(BMP1.GetHeigth()+2), 4*(BMP1.GetHeigth()+2), 24 );
//BUENA   bmpman BMP24bppBIG(  (((((5*(BMP1.GetHeigth()+2)))/100)+1)*100), (((((3*(BMP1.GetHeigth()+2)))/100)+1)*100), 24 );

   //HERE THE FUNCTIONS
   Htile1( BMP1, BMP2, BMP24bpp1, trans_area, factor );
   Htile2( BMP1, BMP2, BMP24bpp2, trans_area, factor );
   Htile3( BMP1, BMP2, BMP24bpp3, trans_area, factor );
   Htile4( BMP1, BMP2, BMP24bpp4, trans_area, factor );
   Htile5( BMP1, BMP2, BMP24bpp5, trans_area, factor );

   Htile7( BMP1, BMP2, BMP24bpp7, trans_area, factor );
   Htile8( BMP1, BMP2, BMP24bpp8, trans_area, factor );
   HtileA( BMP1, BMP2, BMP24bppA, trans_area, factor );
   HtileB( BMP1, BMP2, BMP24bppB, trans_area, factor );
   HtileC( BMP1, BMP2, BMP24bppC, trans_area, factor );
   HtileD( BMP1, BMP2, BMP24bppD, trans_area, factor );
   HtileE( BMP1, BMP2, BMP24bppE, trans_area, factor );

   int width_tile, minus_sep = 2, total_cut;
   int nhor, nver, acum1=0, acum2=0, nver_aux, nhor_aux;
   int i,j,i_aux,j_aux;

   width_tile = BMP1.GetWidth();
   total_cut = width_tile + minus_sep;

   nver = ( BMP24bppBIG.GetWidth() / total_cut);

            nver_aux = 0;
            nhor_aux = 0;
            while( nver > 0)
            {
                nhor = ( BMP24bppBIG.GetHeigth() / total_cut );
                while( nhor > 0)
                {
                     acum1 = 0;
                     i_aux = 0;
                     for ( i = (total_cut*(acum1+nhor_aux)); i < (((acum1+nhor_aux)*total_cut)+(width_tile)); i++ )
                     {
                         acum2 = 0;
                         j_aux = 0;
                         for ( j = (total_cut*(acum2+nver_aux)); j < (((acum2+nver_aux)*total_cut)+(width_tile)); j++ )
                         {

                   switch( nhor_aux )
                   {
                           case 0:
                               switch( nver_aux )
                               {
//[0,x]
                                   case 0:
//                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp0.GetRGBdata( i_aux, j_aux ));
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP1.GetClr( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp1.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp2.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp3.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   default:
                                   break;
                               }
                           break;
//[1,x]
                           case 1:
                               switch( nver_aux )
                               {
                                   case 0:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp4.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp5.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
//6                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp6.GetRGBdata( i_aux, j_aux ));
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP1.GetClr( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp7.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   default:
                                   break;
                               }
                           break;
//[2,x]
                           case 2:
                               switch( nver_aux )
                               {
                                   case 0:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp8.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
//9                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp8.GetRGBdata( i_aux, j_aux ));
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP1.GetClr( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppA.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppB.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   default:
                                   break;
                               }
                           break;
//[3,x]
                         case 3:
                               switch( nver_aux )
                               {
                                   case 0:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppC.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppD.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppE.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
//f                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppF.GetRGBdata( i_aux, j_aux ));
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP2.GetClr( i_aux, j_aux ));
                                   break;
                                   default:
                                   break;
                               }
                           break;

                           default:
                           break;
                   }
                         j_aux++;
                         }
                         acum2++;
                         i_aux++;
                     }
                     acum1++;
                     nhor--;
                     nhor_aux++;

                }

                nhor_aux = 0;
                nver--;
                nver_aux++;
            }


   savename = (string_1.substr( 0, 4 )) + (string_2.substr( 0, 4 ));
   savename = savename + ".bmp";

   BMP24bppBIG.LoadBMPHeader();
   BMP24bppBIG.SaveBMP(savename.c_str());
   cout << "Saved as " << savename << endl;


/*

   BMP24bpp1.LoadBMPHeader();
   BMP24bpp1.SaveBMP("Htile1.bmp");
   BMP24bpp2.LoadBMPHeader();
   BMP24bpp2.SaveBMP("Htile2.bmp");
   BMP24bpp3.LoadBMPHeader();
   BMP24bpp3.SaveBMP("Htile3.bmp");
   BMP24bpp4.LoadBMPHeader();
   BMP24bpp4.SaveBMP("Htile4.bmp");
   BMP24bpp5.LoadBMPHeader();
   BMP24bpp5.SaveBMP("Htile5.bmp");
   BMP24bpp7.LoadBMPHeader();
   BMP24bpp7.SaveBMP("Htile7.bmp");
   BMP24bpp8.LoadBMPHeader();
   BMP24bpp8.SaveBMP("Htile8.bmp");
   BMP24bppA.LoadBMPHeader();
   BMP24bppA.SaveBMP("Htilea.bmp");
   BMP24bppB.LoadBMPHeader();
   BMP24bppB.SaveBMP("Htileb.bmp");
   BMP24bppC.LoadBMPHeader();
   BMP24bppC.SaveBMP("Htilec.bmp");
   BMP24bppD.LoadBMPHeader();
   BMP24bppD.SaveBMP("Htiled.bmp");
   BMP24bppE.LoadBMPHeader();
   BMP24bppE.SaveBMP("Htilee.bmp");
*/

//   system("PAUSE");

   return 1;
}

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


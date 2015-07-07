/*
   Name: testmain.cpp, MAIN FUNCTION FOR THE AIRBASE PROGRAM
   Author: Tomas Garcia-Pozuelo Barrios
   Description: AUTOMATIC ALL (DESIGNED FOR RIVERS) TILE MAKER FOR FALCON 4.0
   Date: Started 03/12/2002
   Copyright: 
*/

//INCLUDE´s
#include "..\bmpmanclass.h"
#include <math.h>

//PROTOTIPES
void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int perctg1, int dist );
int String2Int( string s );
bool ShouldBeTrans( int dist, int x, int y, bmpman &BMPTILE, float &totaldist );


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
   bmpman BMP1, BMP2;
   string savename, string_1, string_2, s_aux;

   //**TEMPORAL**
   int perctg1, dist;



     ifstream f;
     int nfiles, total, inic;
     int i, j, j_aux;

     cout << "*********************************************************************\n";
     cout << "*     AUTO TILE MAKER WITH 128x128, 256x256 AND 512x512 SUPPORT     *\n";
     cout << "*********************************************************************" << endl << endl << "By Com_GP (Com_GP@yahoo.com) 06/12/2002" << endl << endl;
     cout << "SCRIPT DRIVEN, the script should be in the same folder and its\ncalled bmplist.txt\n";

     f.open("bmplist.txt");
     if(!f){
           cout << "ERROR OPENING THE SCRIPT FILE" << endl;
           system("PAUSE");
     }
     else{

      getline( f, s_aux);
      nfiles = String2Int( s_aux );
      cout << endl << "NUMBER OF BMPs ---------> " << nfiles << endl;

      getline( f, string_1 );
      BMP1.LoadBMP(string_1.c_str());
      cout << endl << "WIDTH OF THE BMPs ---------> " << BMP1.GetWidth() << endl;

      getline( f, s_aux);
      perctg1 = String2Int( s_aux );
      cout << endl << "PERCENTAGE OF THE TRANSITION ---------> " << perctg1 << endl;

      getline( f, s_aux);
      dist = String2Int( s_aux );
      cout << endl << "RATIUS DISTANCE OF THE TRANSITION ---------> " << dist << endl;

      cout << endl;

      getline( f, savename );

      total = (BMP1.GetWidth()+2)*nfiles;
      bmpman BMP24bppBIG((BMP1.GetWidth()+2)*nfiles, (BMP1.GetWidth()+2), 24);
      cout << "Procesing .";
      while( nfiles > 0 ){
          cout << ".";
          getline( f, string_2 );
          BMP2.LoadBMP(string_2.c_str());

   //Same Width and Height
   if( BMP1.GetWidth() != BMP1.GetHeigth() ){
       cout << "\nERROR, THE FIRST BITMAP HEIGHT IS NOT THE SAME AS THE WIDTH" << endl;
       system("PAUSE");
       return 0;
   }

   //Same Width and Height
   if( BMP2.GetWidth() != BMP2.GetHeigth() ){
       cout << "\nERROR, THE SECOND BITMAP HEIGHT IS NOT THE SAME AS THE WIDTH" << endl;
       system("PAUSE");
       return 0;
   }

   //The bmp1 load right
   if( BMP1.GetWidth() == 0 ){
       cout << "\nERROR, THE FIRST BMP IS NOT RIGHT LOADED (probably wrong typing)" << endl;
       system("PAUSE");
       return 0;
   }

   //The bmp2 load right
   if( BMP2.GetWidth() == 0 ){
       cout << "\nERROR, THE SECOND BMP IS NOT RIGHT LOADED (probably wrong typing)" << endl;
       system("PAUSE");
       return 0;
   }

   //Same Width in the two tiles
   if( BMP1.GetWidth() != BMP2.GetWidth() ){
       cout << "\nERROR, THE TILES DONT HAVE THE SAME WIDTH OR HEIGHT" << endl;
       system("PAUSE");
       return 0;
   }

   bmpman BMP24bpp1(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   Htile( BMP1, BMP2, BMP24bpp1, perctg1, dist );

//****************

   inic = (total - (BMP1.GetWidth()+2)*nfiles);
   for( i = 0; i < BMP1.GetWidth(); i++ ){
      j_aux = 0;
      for( j = inic; j < (inic+BMP1.GetWidth()); j++ ){
           BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp1.GetRGBdata( i, j_aux ));
           j_aux++;
      }
   }

//****************
          nfiles --;
      }
     BMP24bppBIG.LoadBMPHeader();
     BMP24bppBIG.SaveBMP(savename.c_str());
     f.close();
     }

   cout << "\n\nName to save -->";
   cout << savename << endl;

   system("PAUSE");
   return 0;
}



void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int perctg1, int dist )
{
   int x, y;
   int changpixels1, changpixels2;
   RGBQUAD rgb, rgb_aux, rgb_auxRES;
   float totaldist;

   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           rgb = BMP2.GetClr( x, y );
           if( ( char(rgb.rgbRed) != char(255) ) && (char(rgb.rgbGreen) != char(255) ) && (char(rgb.rgbBlue) != char(255) )){
               if( ShouldBeTrans( dist, x, y, BMP2, totaldist )){
                   //**Transitions**
                   rgb_aux = BMP1.GetClr( x, y );
                   rgb_auxRES.rgbRed = char( int( ( int(rgb_aux.rgbRed)* (((perctg1*totaldist)/100)) ) + ( int(rgb.rgbRed)*((100-(perctg1*totaldist))/100))  ));
                   rgb_auxRES.rgbGreen = char(int((int(rgb_aux.rgbGreen)*(((perctg1*totaldist)/100)) )+ ( int(rgb.rgbGreen)*((100-(perctg1*totaldist))/100))  ));
                   rgb_auxRES.rgbBlue = char(int((int(rgb_aux.rgbBlue)*(((perctg1*totaldist)/100)) )+ ( int(rgb.rgbBlue)*((100-(perctg1*totaldist))/100))  ));
                   BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );

               }
               else{
                   BMP24bpp.EnterRGBdata( x, y, rgb );//No es blanco
               }
           }
           else{
               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
           }
       }
   }


}

bool ShouldBeTrans( int dist, int x, int y, bmpman &BMPTILE, float &totaldist )
{
     bool should = false;
     int i, j;
     RGBQUAD rgb;

     for( i = (x-dist); ((i <= (x+dist))&&(!should)); i++ ){ //no seguro del = de en medio
          for( j = (y-dist); ((j <= (y+dist))&&(!should)); j++ ){ //no seguro del = de en medio
               if(!( (i > BMPTILE.GetWidth()-1) || (j > BMPTILE.GetHeigth()-1) || (i < 0 ) || (j < 0 ))){
                rgb = BMPTILE.GetClr( i, j );
                if( ( char(rgb.rgbRed) != char(255) ) && (char(rgb.rgbGreen) != char(255) ) && (char(rgb.rgbBlue) != char(255) )){
                    totaldist = 0.0;
                }
                else{
                    should = true;
                    totaldist = sqrt( ((i-x)*(i-x))+((j-y)*(j-y)) );
                }

               }
          }
     }

     totaldist = ( totaldist / sqrt(2*(dist*dist)) );
     if ((totaldist > 1) || (totaldist < 0 ))
     cout << "totaldist erroneo\n";
     return should;
}


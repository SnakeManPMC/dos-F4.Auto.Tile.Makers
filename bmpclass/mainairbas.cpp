/*
   Name: testmain.cpp, MAIN FUNCTION FOR THE AIRBASE PROGRAM
   Author: Tomas Garcia-Pozuelo Barrios
   Description: AUTOMATIC AIRBASE TILE MAKER FOR FALCON 4.0
   Date: Started 03/12/2002
   Copyright: 
*/

//INCLUDE´s
#include "bmpmanclass.h"
#include <math.h>

//PROTOTIPES
void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp );
int String2Int( string s );


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



     ifstream f;
     int nfiles, total, inic;
     int i, j, j_aux;

     cout << "*********************************************************************\n";
     cout << "*     AUTO AIRBASE MAKER WITH 128x128 (AND 256x256 not yet) SUPPORT *\n";
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

      getline( f, savename );

      total = (BMP1.GetWidth()+2)*nfiles;
      bmpman BMP24bppBIG((BMP1.GetWidth()+2)*nfiles, (BMP1.GetWidth()+2), 24);

      while( nfiles > 0 ){
          getline( f, string_2 );

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

   bmpman BMP24bpp1(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   Htile( BMP1, BMP2, BMP24bpp1 );

//   BMP24bpp1.LoadBMPHeader();
//   BMP24bpp1.SaveBMP(savename.c_str());
//****************

   inic = (total - (BMP1.GetWidth()+2)*nfiles);
   for( i = 0; i < BMP1.GetWidth(); i++ ){
      j_aux = 0;
      for( j = inic; j < (inic+BMP1.GetWidth()); j++ ){
//           cout << "i = " << i << " j = " << j << " j_aux = " << j_aux << endl;
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

   cout << "Name to save -->";
   cout << savename << endl;

   system("PAUSE");
   return 0;
}



void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb;

   //COPY BMP2 on BMP24bpp

   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           rgb = BMP2.GetClr( x, y );
           if( ( char(rgb.rgbRed) != char(255) ) && (char(rgb.rgbGreen) != char(255) ) && (char(rgb.rgbBlue) != char(255) ))
               BMP24bpp.EnterRGBdata( x, y, rgb );//No es blanco
           else
               BMP24bpp.EnterRGBdata( x, y, BMP1.GetClr( x, y ) );
       }
   }


}

//HtileEnd( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, int fact, char tipe )

/*
int MakeTransition( string string_1, string string_2, int cin_aux, int factor, bool rail )
{

   //VARIABLES




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
   bmpman BMP24bpp6(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp7(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp8(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bpp9(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppA(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppB(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppC(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppD(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppE(BMP1.GetWidth(), BMP1.GetWidth(), 24);
   bmpman BMP24bppF(BMP1.GetWidth(), BMP1.GetWidth(), 24);


   //cambiar tamaños para contemplar 256x256
//   bmpman BMP24bppBIG;//(5*(BMP1.GetHeigth()+2)), (4*(BMP1.GetHeigth()+2)), 24 );
//   BMP24bppBIG.LoadBMP("bmpbase.dat");
                                                                         //todavia tengo que cambiarlo
   bmpman BMP24bppBIG(  (((((5*(BMP1.GetHeigth()+2)))/100)+1)*100), (((((3*(BMP1.GetHeigth()+2)))/100)+1)*100), 24 );

   //HERE THE FUNCTIONS
   Htile( BMP1, BMP2, BMP24bpp3, factor, '3', rail );
   Htile( BMP1, BMP2, BMP24bpp6, factor, '6', rail );
   Htile( BMP1, BMP2, BMP24bpp9, factor, '9', rail );
   Htile( BMP1, BMP2, BMP24bppC, factor, 'c', rail );

   Htile( BMP1, BMP2, BMP24bpp5, factor, '5', rail );
   Htile( BMP1, BMP2, BMP24bpp7, factor, '7', rail );
   Htile( BMP1, BMP2, BMP24bppA, factor, 'a', rail );
   Htile( BMP1, BMP2, BMP24bppB, factor, 'b', rail );
   Htile( BMP1, BMP2, BMP24bppD, factor, 'd', rail );
   Htile( BMP1, BMP2, BMP24bppE, factor, 'e', rail );
   Htile( BMP1, BMP2, BMP24bppF, factor, 'f', rail );



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
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp7.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppB.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp8.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppC.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 4:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp4.GetRGBdata( i_aux, j_aux ));
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
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppD.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppE.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppA.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bppF.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 4:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp5.GetRGBdata( i_aux, j_aux ));
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
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp6.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 1:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp9.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 2:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp2.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 3:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp3.GetRGBdata( i_aux, j_aux ));
                                   break;
                                   case 4:
                                       BMP24bppBIG.EnterRGBdata( i, j, BMP24bpp1.GetRGBdata( i_aux, j_aux ));
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

   system("PAUSE");

   return 1;
}
*/

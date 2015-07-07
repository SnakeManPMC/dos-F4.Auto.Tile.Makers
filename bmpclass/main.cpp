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
void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, char tipe, bool rail, bool curves, float wr );
void HtileEnd( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, char tipe, bool rail, float wr );
int MakeTransition( string string_1, string string_2, int cin_aux, int factor, bool rail, bool curves, float wr );
int String2Int( string s );
int CalcDist( int x, int y, char tipe, bmpman &BMP1, bmpman &BMP2, bool curves, float wr );
int CalcPerctg( int x, int y, char tipe, bmpman &BMP1, bmpman &BMP2 );


int main()
{

     int x, y, h, w;
     int nfiles, smooth;//smooth no se usa todavia
     int factor, factor1;
     int intaux;
     bool rail;
     bool curves;
     float wroad;

     string s_aux;
     string file1, file2;

     ifstream f;


     cout << "***********************************************************\n";
     cout << "*     AUTO ROAD MAKER WITH 128x128 AND 256x256 SUPPORT    *\n";
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
      cout << endl << "NUMBER OF BMPs ---------> " << nfiles << endl;
      getline( f, s_aux);
      rail = String2Int( s_aux );
      cout << "RAIL EFFECT ------------> " << rail << endl;
      getline( f, s_aux);
      curves = String2Int( s_aux );
      cout << "CURVES EFFECT ----------> " << curves << endl << endl;
      getline( f, s_aux);
      intaux = String2Int( s_aux );
      wroad = (float(intaux)/100.0);
      cout << "PERCENTAGE EFFECT ------> " << intaux << endl << endl;



      rail = !rail;

      system("PAUSE");
      getline( f, file1);
      while( nfiles > 0 ){
          getline( f, file2 );
          if( MakeTransition( file1, file2, factor, factor1, rail, curves, wroad ) == 0 )
              return 0;
          nfiles --;
      }

     f.close();
     }
   return 0;
}



void Htile( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, char tipe, bool rail, bool curves, float wr )
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
   cout << "Htile(" << tipe << ")";

        for( x = 0; x < BMP1.GetWidth(); x++){
             for( y = 0; y < BMP1.GetWidth(); y++){
                      dist = CalcDist( x, y, tipe, BMP1, BMP2, curves, wr );

                      if( rail ){
                       perctg1 = 100;
                       if( (((BMP2.GetWidth()/2)) >= dist) || ( dist > (BMP2.GetWidth()/2)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                      }


                       perctg1 = 80;
                       if( (((BMP2.GetWidth()/2)-(1*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(1*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       perctg1 = 35;
                       if( (((BMP2.GetWidth()/2)-(3*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(3*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }

                       perctg1 = 15;
                       if( (((BMP2.GetWidth()/2)-(5*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(5*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }

                       perctg1 = 0;
                       if( !(((BMP2.GetWidth()/2)-(5*wr)) < dist) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( !(((BMP2.GetWidth()/2)+(5*wr)) > dist) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }


             }
        }

   cout << "........";
   cout << "Done\n";
}

void HtileEnd( bmpman &BMP1, bmpman &BMP2, bmpman &BMP24bpp, char tipe, bool rail, float wr )
{
   int x, y;
   int perctg1, dist, changpixels1, changpixels2;
   RGBQUAD rgb_aux1, rgb_aux2, rgb_auxRES;
   bool cur_aux;

   //COPY BMP2 on BMP24bpp
   for( x = 0; x < BMP2.GetWidth(); x++){
       for( y = 0; y < BMP2.GetWidth(); y++){
           BMP24bpp.EnterRGBdata( x, y, BMP2.GetClr( x, y ) );
       }
   }
   cout << "Htile(" << tipe << ")";

        for( x = 0; x < BMP1.GetWidth(); x++){
             for( y = 0; y < BMP1.GetWidth(); y++){
                      dist = CalcDist( x, y, tipe, BMP1, BMP2, cur_aux, wr );


                      if( rail ){
                       perctg1 = 0;
                       perctg1 = CalcPerctg( x, y, tipe, BMP1, BMP2 );
                       perctg1 =  100 - perctg1;

                       if( perctg1 < 0 ) perctg1 = 0;

                       if( (((BMP2.GetWidth()/2)) >= dist) || ( dist > (BMP2.GetWidth()/2)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                      }



                       perctg1 = 0;
                       perctg1 = CalcPerctg( x, y, tipe, BMP1, BMP2 );
                       perctg1 =  80 - perctg1;

                       if( perctg1 < 0 ) perctg1 = 0;

                       if( (((BMP2.GetWidth()/2)-(1*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(1*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }

                       perctg1 = 0;
                       perctg1 = CalcPerctg( x, y, tipe, BMP1, BMP2 );
                       perctg1 =  35 - perctg1;

                       if( perctg1 < 0 ) perctg1 = 0;

                       if( (((BMP2.GetWidth()/2)-(3*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(3*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }


                       perctg1 = 0;
                       perctg1 = CalcPerctg( x, y, tipe, BMP1, BMP2 );
                       perctg1 =  15 - perctg1;

                       if( perctg1 < 0 ) perctg1 = 0;

                       if( (((BMP2.GetWidth()/2)-(5*wr)) >= dist) || ( dist > (BMP2.GetWidth()/2)+(5*wr)) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }

                       perctg1 = 0;
                       if( !(((BMP2.GetWidth()/2)-(5*wr)) < dist) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }
                       if( !(((BMP2.GetWidth()/2)+(5*wr)) > dist) ){
                           rgb_aux1 = BMP1.GetClr( x, y );
                           rgb_aux2 = BMP2.GetClr( x, y );
                           rgb_auxRES.rgbRed = char(int(((int((rgb_aux1.rgbRed))*perctg1)/100)+((int((rgb_aux2.rgbRed))*(100-perctg1))/100)));
                           rgb_auxRES.rgbGreen = char(int(((int((rgb_aux1.rgbGreen))*perctg1)/100)+((int((rgb_aux2.rgbGreen))*(100-perctg1))/100)));
                           rgb_auxRES.rgbBlue = char(int(((int((rgb_aux1.rgbBlue))*perctg1)/100)+((int((rgb_aux2.rgbBlue))*(100-perctg1))/100)));
                           BMP24bpp.EnterRGBdata( x, y, rgb_auxRES );
                       }


             }
        }
   cout << "........";
   cout << "Done\n";
}



int MakeTransition( string string_1, string string_2, int cin_aux, int factor, bool rail, bool curves, float wr )
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
   Htile( BMP1, BMP2, BMP24bpp3, '3', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bpp6, '6', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bpp9, '9', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppC, 'c', rail, curves, wr );

   Htile( BMP1, BMP2, BMP24bpp5, '5', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bpp7, '7', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppA, 'a', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppB, 'b', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppD, 'd', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppE, 'e', rail, curves, wr );
   Htile( BMP1, BMP2, BMP24bppF, 'f', rail, curves, wr );

   HtileEnd( BMP1, BMP2, BMP24bpp4, '4', rail, wr );
   HtileEnd( BMP1, BMP2, BMP24bpp1, '1', rail, wr );
   HtileEnd( BMP1, BMP2, BMP24bpp8, '8', rail, wr );
   HtileEnd( BMP1, BMP2, BMP24bpp2, '2', rail, wr );


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

int CalcDist( int x, int y, char tipe, bmpman &BMP1, bmpman &BMP2, bool curves, float wr )
{
    int dist;

    switch( tipe )
    {
           case '9'://Curve
                if ( curves ) dist = int(sqrt( (x*x) + (y*y) ));
                else dist = int( (x+y) );
                break;
           case '3'://Curve
                if ( curves ) dist = int(sqrt( (x*x) + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) ));
                else dist = int( (x+(BMP1.GetWidth()-y)) );
                break;
           case '6'://Curve
                if ( curves ) dist = int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + ((BMP1.GetWidth()-y)*(BMP1.GetWidth()-y)) ));
                else dist = int( ((BMP1.GetWidth()-x)+(BMP1.GetWidth()-y)) );
                break;
           case 'c'://Curve
                if ( curves ) dist = int(sqrt( ((BMP1.GetWidth()-x)*(BMP1.GetWidth()-x)) + (y*y) ));
                else dist = int( ((BMP1.GetWidth()-x)+y) );
                break;
           case '5'://Line
                dist = y;
                break;
           case 'a'://Line
                dist = x;
                break;
           case 'f'://Cross
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (x >= (BMP1.GetWidth()/2)-(wr*5)) && (x <= (BMP1.GetWidth()/2)+(wr*5)) )
                    if( (y >= (BMP1.GetWidth()/2)-(wr*5)) && (y <= (BMP1.GetWidth()/2)+(wr*5)) )
                        dist = x;
                break;
           case 'b'://3way Cross
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) && (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (x >= (BMP1.GetWidth()/2)-(wr*5)) && (x <= (BMP1.GetWidth()/2)+(wr*5)) )
                    if( (y >= (BMP1.GetWidth()/2)-(wr*5)) && (y <= (BMP1.GetWidth()/2)+(wr*5)) )
                        dist = x;
                break;
           case '7'://3way Cross
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) && (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (x >= (BMP1.GetWidth()/2)-(wr*5)) && (x <= (BMP1.GetWidth()/2)+(wr*5)) )
                    if( (y >= (BMP1.GetWidth()/2)-(wr*5)) && (y <= (BMP1.GetWidth()/2)+(wr*5)) )
                        dist = y;
                break;
           case 'e'://3way Cross
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) && (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (x >= (BMP1.GetWidth()/2)-(wr*5)) && (x <= (BMP1.GetWidth()/2)+(wr*5)) )
                    if( (y >= (BMP1.GetWidth()/2)-(wr*5)) && (y <= (BMP1.GetWidth()/2)+(wr*5)) )
                        dist = x;
                break;
           case 'd'://3way Cross
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = x;
                if( (((BMP2.GetWidth()/2)-(wr*5)) >= x) || ( x > (BMP2.GetWidth()/2)+(wr*5)) && (((BMP2.GetWidth()/2)-(wr*5)) >= y) || ( y > (BMP2.GetWidth()/2)+(wr*5)) )
                    dist = y;
                if( (x >= (BMP1.GetWidth()/2)-(wr*5)) && (x <= (BMP1.GetWidth()/2)+(wr*5)) )
                    if( (y >= (BMP1.GetWidth()/2)-(wr*5)) && (y <= (BMP1.GetWidth()/2)+(wr*5)) )
                        dist = y;
                break;

           case '4'://Line
                dist = y;
                break;
           case '1'://Line
                dist = y;
                break;
           case '8'://Line
                dist = x;
                break;
           case '2'://Line
                dist = x;
                break;

           default:
           break;
    }
    return dist;
}


int CalcPerctg( int x, int y, char tipe, bmpman &BMP1, bmpman &BMP2 )
{
    int Perctg;

    switch( tipe )
    {
           case '1'://Line
                if( x >= 0 ) Perctg = 0;
                if( x > 64 ) Perctg = 33;
                if( x > 128 ) Perctg = 66;
                if( x > 192 ) Perctg = 99;
                break;
           case '4'://Line
                if( x >= 0 ) Perctg = 99;
                if( x > 64 ) Perctg = 66;
                if( x > 128 ) Perctg = 33;
                if( x > 192 ) Perctg = 0;
                break;
           case '2'://Line
                if( y >= 0 ) Perctg = 99;
                if( y > 64 ) Perctg = 66;
                if( y > 128 ) Perctg = 33;
                if( y > 192 ) Perctg = 0;
                break;
           case '8'://Line
                if( y >= 0 ) Perctg = 0;
                if( y > 64 ) Perctg = 33;
                if( y > 128 ) Perctg = 66;
                if( y > 192 ) Perctg = 99;
                break;


           default:
           break;
    }
    return Perctg;
}


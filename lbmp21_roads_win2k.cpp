
// *************************************************************
//                  11/23/2002 Tomas GPozuelo
//                    <Com_GP@hotmail.com>
// *************************************************************

#include <windows.h>
#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

// **********
// class CRaster
//   - Generic class for BMP raster images.
class CRaster {
	public:
		int Width,Height;		// Dimensions
		int BPP;				// Bits Per Pixel.
		char ** Raster;			// Bits of the Image.
		RGBQUAD * Palette;		// RGB Palette for the image.
		int BytesPerRow;		// Row Width (in bytes).
		BITMAPINFO * pbmi;		// BITMAPINFO structure

		// Member functions (defined later):
		void LoadBMP ( const char * szFile);
  		void SaveBMP ( const char * szFile, CRaster bmp, int temp_i );
//		int GDIPaint (HDC hdc,int x,int y);
};

int String2Int( string s );
// **********
// CRaster::LoadBMPFile (FileName);
//   - loads a BMP file into a CRaster object
//   * supports non-RLE-compressed files of 1, 2, 4, 8 & 24 bits-per-pixel
void CRaster::LoadBMP ( const char * szFile)
{
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// Open file.
	ifstream bmpfile (szFile , ios::in | ios::binary);

	if (! bmpfile.is_open())
    {
        cout << "ERROR OPENING FILE" << endl;		// Error opening file
        system("PAUSE");
    }

	// Load bitmap fileheader & infoheader
	bmpfile.read ((char*)&bmfh,sizeof (BITMAPFILEHEADER));
	bmpfile.read ((char*)&bmih,sizeof (BITMAPINFOHEADER));

	// Check filetype signature
	if (bmfh.bfType!='MB')
    {
        cout << "ERROR FILE IS NOT BMP" << endl;		// File is not BMP
        system("PAUSE");
    }
	// Assign some short variables:
	BPP=bmih.biBitCount;      //bits per pixel
    if ( BPP != 8 )
    {
         cout << "ERROR THE BMP FILE SHOULD BE 8 BPP" << endl;
         system("PAUSE");
    }
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

//	Raster= new char[BytesPerRow*Height];

//COMIENZO
    int i, bitwidth;

    bitwidth = ((BytesPerRow / sizeof(char)));
    if((BytesPerRow % sizeof(char) > 0 ))
         bitwidth++;
//    cout << bitwidth << endl;
//    system("PAUSE");
//start
	//se reserva memoria para las filas
	Raster = new char*[Height];

	//se reserva memoria para las columnas
	for( i = 0; i <= Width; i++ )
		Raster[i] = new char[Width];
//end     FIN


                       //El problema ESTA AQUI!!!!
	// (if height is positive the bmp is bottom-up, read it reversed)
	if (bmih.biHeight>0)                               //NO SE DE ESTAS LINEAS
		for (int n=Height-1;n>=0;n--)
			bmpfile.read (Raster[n],BytesPerRow);
	else                                               //NO SE DE ESTAS LINEAS
		bmpfile.read (Raster,BytesPerRow*Height);      //NO SE DE ESTAS LINEAS

	// so, we always have a up-bottom raster (that is negative height for windows):
	pbmi->bmiHeader.biHeight=-Height;

	bmpfile.close();


    cout << "SIZE OF THE BMP ---> " << ( sizeof(bmfh) + sizeof(BITMAPINFO) + 1024 + (Width*Height)-4) << endl;
//    system("PAUSE");
}

void SaveBMP ( const char * szFile, CRaster bmp, int temp_i )
{
    BITMAPFILEHEADER bmfh;

    bmfh.bfType='MB',                                 //PALETTE
    bmfh.bfSize = ( sizeof(bmfh) + sizeof(BITMAPINFO) + 1024 + (bmp.Width*bmp.Height));//quizas -4
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits =  1078;


	BITMAPINFOHEADER bmih = bmp.pbmi->bmiHeader;

    // Open file.
	ofstream bmpfile (szFile , ios::out | ios::binary);

	if (! bmpfile.is_open())
        cout << "ERROR SAVING FILE" << endl;		// Error opening file

	//Save bitmap fileheader & infoheader
	bmpfile.write ((char*)&bmfh,sizeof (BITMAPFILEHEADER));
	bmpfile.write ((char*)&bmih,sizeof (BITMAPINFOHEADER));

    int i, j;
    for( i = 0; i < 256; i++ )
    {
    bmpfile.write ((char*)&bmp.Palette[i], sizeof(char)*4);
//    cout << i << endl;
    }

    //Save the data
//    cout << bmp.Height << " " << bmp.Width << endl;
//    system("PAUSE");


    for( i = (bmp.Height-1); i >= 0; i-- )
        for( j = 0; j < bmp.Width+temp_i; j++ )
        {
             bmpfile.write ((char*)&bmp.Raster[i][j], sizeof(char));
//             cout << i << ", " << j << endl;
        }

	bmpfile.close();
}

int main()
{

    static CRaster bmp, bmp_temp;
    int width_tile, minus_sep, total_cut, nhor, nver, acum1=0, acum2=0, nver_aux, nhor_aux, i,j,i_aux,j_aux;


    //TEMPORAL
    int x_aux = 0;
    int intnsig_aux;

    cout << "********************************\n";
    cout << "*  CUTTER WITH FALCON SUPPORT  *\n";
    cout << "********************************" << endl << endl << "By Com_GP (Com_GP@yahoo.com) 06/12/2002" << endl << "SUPPORTS 128x128 AND 256x256 TILES WITH 2 PIXELS OF SEPARATION" << endl << endl;
    system("PAUSE");

    RGBQUAD *rgbq_aux;        //Points wherever the palette starts
//    char ** Raster_aux, ** Raster_main;        //Points wherever the image bytes start
//    char cc [256];
//    char string1 [12];

    string cc, string1;

    string s_aux;
    int nfiles;
    int menus = 0;

  ofstream pr;
  ifstream ifs;

  pr.open("texturebin.txt");
  if(!pr)
         cout << "ERROR GUARDANDO EN EL FICHERO" << endl;
  else
  {

  ifs.open("list8bpp.txt");
  if(!ifs)
      cout << "ERROR OPENING THE SCRIPT FILE" << endl;
  else{

  getline( ifs, s_aux);
  nfiles = String2Int( s_aux );
  cout << "NUMBER OF FILES = " << nfiles << endl;


while( nfiles > 0 )
{
  getline( ifs, cc );
  getline( ifs, string1 );

  getline( ifs, s_aux );
  menus = String2Int( s_aux );

  //Ask for the name of the bmp file
  printf ("The file opened is ------> ");
  cout << cc;
  printf ("\n\n");

  bmp.LoadBMP (cc.c_str());

        pr << "# Start Set X has X tiles, Terrain type 4" << endl << "SET 4" << endl;


            //The width of the tile
            cout << "The width and height (its a square) of the tile is ---> ";
            switch( menus )
            {
                 case 1:
                     cout << "128x128\n\n";
                     width_tile = 128;
                 break;
                 case 2:
                     cout << "256x256\n\n";
                     width_tile = 256;
                 break;
                 default:
                     cout << "ERROR, MUST BE BETWEEN 1 AND 2, CHECK THE list8bpp.txt FILE" << endl;
                     system("PAUSE");
                 break;
            }

            //Separation between tiles
            cout << "Separation between tiles = ";
            minus_sep = 2;
            cout << minus_sep << endl;



            //bmp_temp = bmp ASIGNACIONES DE TODOS LOS CAMPOS INCLUIDA LA PALETA
            bmp_temp.Width = width_tile;
            bmp_temp.Height = width_tile;
            bmp_temp.BPP = bmp.BPP;
            bmp_temp.Palette = bmp.Palette;
            bmp_temp.pbmi = bmp.pbmi;
            bmp_temp.pbmi->bmiHeader.biHeight = (width_tile);
            bmp_temp.pbmi->bmiHeader.biWidth = (width_tile);

            //new temp memory for the temp tile

            total_cut = width_tile + minus_sep;

            //HASTA AQUI BIEN 23/11/02 01:19
            bmp_temp.Raster = new char*[total_cut];
            for( i = 0; i <= total_cut; i++ )
		          bmp_temp.Raster[i] = new char[total_cut];

            //HASTA AQUI BIEN 23/11/02 01:23
            nver = (bmp.Width / total_cut);
//            nver = (bmp.Height /total_cut);

            cout << "Name of the FILE in which is gonna be saved";
            cout << string1;
            cout << endl;
//            string1 += "Hname**";
//            string1 = string1 + ".bmp";

            int temp_i = 0;
            cout << "CORRECTOR FOR INCLINATION = " << temp_i << endl;


            nver_aux = 0;
            nhor_aux = 0;
            while( nver > 0)
            {
                nhor = ( bmp.Height / total_cut );
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
                               bmp_temp.Raster[i_aux][j_aux] = bmp.Raster[i][j];//Aqui hay algun problema
//                               cout <<"[" << i_aux << "][" << j_aux <<"]," <<"  [" << i << "][" << j <<"]" << endl;
                               j_aux++;
                         }
//                         system("PAUSE");
                         acum2++;
                         i_aux++;
                     }
                     acum1++;
//                     nver_aux = 0;

//                     cout <<"[" << nver << "][" << nhor <<"]" << endl;
//                     cout <<"[" << nver_aux << "][" << nhor_aux <<"]" << endl << endl;

                     nhor--;
                     nhor_aux++;



                     x_aux++;

                   switch( nhor_aux - 1 )
                   {
                           case 0:
                               switch( nver_aux )
                               {
//[0,x]
                                   case 0:
                                       string1[7] = '7';
                                       cout << "TILE 7 ---> DONE" << endl;
                                   break;
                                   case 1:
                                       string1[7] = 'B';
                                       cout << "TILE b ---> DONE" << endl;
                                   break;
                                   case 2:
                                       string1[7] = '8';
                                       cout << "TILE 8 ---> DONE" << endl;
                                   break;
                                   case 3:
                                       string1[7] = 'C';
                                       cout << "TILE c ---> DONE" << endl;
                                   break;
                                   case 4:
                                       string1[7] = '4';
                                       cout << "TILE 4 ---> DONE" << endl;
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
                                       string1[7] = 'D';
                                       cout << "TILE d ---> DONE" << endl;
                                   break;
                                   case 1:
                                       string1[7] = 'E';
                                       cout << "TILE e ---> DONE" << endl;
                                   break;
                                   case 2:
                                       string1[7] = 'A';
                                       cout << "TILE a ---> DONE" << endl;
                                   break;
                                   case 3:
                                       string1[7] = 'F';
                                       cout << "TILE f ---> DONE" << endl;
                                   break;
                                   case 4:
                                       string1[7] = '5';
                                       cout << "TILE 5 ---> DONE" << endl;
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
                                       string1[7] = '6';
                                       cout << "TILE 6 ---> DONE" << endl;
                                   break;
                                   case 1:
                                       string1[7] = '9';
                                       cout << "TILE 9 ---> DONE" << endl;
                                   break;
                                   case 2:
                                       string1[7] = '2';
                                       cout << "TILE 2 ---> DONE" << endl;
                                   break;
                                   case 3:
                                       string1[7] = '3';
                                       cout << "TILE 3 ---> DONE" << endl;
                                   break;
                                   case 4:
                                       string1[7] = '1';
                                       cout << "TILE 1 ---> DONE" << endl;
                                   break;
                                   default:
                                   break;
                               }
                           break;
                           default:
                           break;
                   }


                     //GUARDAR EN FICHERO
                     if( string1[7] != 'x' ){
                     SaveBMP (string1.c_str(), bmp_temp, temp_i );
                     string1[9]='p';
                     string1[10]='c';
                     string1[11]='x';
                     pr << string1 << " 0 0" <<  endl;
                     string1[9]='b';
                     string1[10]='m';
                     string1[11]='p';
                     }
            }

                nhor_aux = 0;
                nver--;
                nver_aux++;
            }
          cout << "Cutting of the set done, 13 files should be done" << endl;
          system("PAUSE");

          pr << "ENDSET" << endl << endl;

nfiles--;
}    //end while
    ifs.close();
    }
  pr.close();
  }
  cout << "A texturebin.txt should be done" << endl;
    return 0;
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

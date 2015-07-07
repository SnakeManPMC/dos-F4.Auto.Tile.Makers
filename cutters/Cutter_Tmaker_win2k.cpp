
// *************************************************************
//                  11/23/2002 Tomas GPozuelo
//                    <Com_GP@hotmail.com>
//                  COMPILER USED: Dev-C++ 4
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
		int LoadBMP ( const char * szFile);
  		void SaveBMP ( const char * szFile, CRaster bmp, int temp_i );
//		int GDIPaint (HDC hdc,int x,int y);
};

int String2Int( string s );
// **********
// CRaster::LoadBMPFile (FileName);
//   - loads a BMP file into a CRaster object
//   * supports non-RLE-compressed files of 8 bits-per-pixel
int CRaster::LoadBMP ( const char * szFile)
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
        return 1;
    }
	// Assign some short variables:
	BPP=bmih.biBitCount;      //bits per pixel
    if ( BPP != 8 )
    {
        cout << "ERROR THE BMP FILE SHOULD BE 8 BPP" << endl;
        system("PAUSE");
        return 1;
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

    int i, bitwidth;

    bitwidth = ((BytesPerRow / sizeof(char)));
    if((BytesPerRow % sizeof(char) > 0 ))
         bitwidth++;

	//se reserva memoria para las filas
	Raster = new char*[Height];

	//se reserva memoria para las columnas
	for( i = 0; i <= Width; i++ )
		Raster[i] = new char[Width];

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

    return 0;
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
    for( i = 0; i < 256; i++ ){
    bmpfile.write ((char*)&bmp.Palette[i], sizeof(char)*4);
    }

    //Save the data
    for( i = (bmp.Height-1); i >= 0; i-- )
        for( j = 0; j < bmp.Width+temp_i; j++ )
        {
             bmpfile.write ((char*)&bmp.Raster[i][j], sizeof(char));
        }

	bmpfile.close();
}

int main()
{

  static CRaster bmp, bmp_temp;
  int width_tile, minus_sep, total_cut, nhor, nver, acum1=0, acum2=0, nver_aux, nhor_aux, i,j,i_aux,j_aux;
  int nfiles_aux;

  int x_aux = 0;
  int intnsig_aux;
  string cc, string1, naming;

  cout << "********************************\n";
  cout << "*  CUTTER WITH FALCON SUPPORT  *\n";
  cout << "********************************" << endl << endl << "By Com_GP (Com_GP@yahoo.com) DATE: " << "23/01/2003" << endl << "SUPPORTS 128x128, 256x256 AND 512x512 TILES WITH 2 PIXELS OF SEPARATION" << endl << endl;
//    system("PAUSE");

  RGBQUAD *rgbq_aux;        //Points wherever the palette starts

  string s_aux;
  int nfiles;
  int menus = 0;

  ofstream pr;
  ifstream ifs;

  pr.open("texturebin.txt");
  if(!pr)
         cout << "ERROR SAVING TEXTURE.BIN, NO MEMORY??" << endl;
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

  //**ESTO**
  getline( ifs, s_aux);
  nfiles_aux = String2Int( s_aux );

  //NAMING PROCEDURE
  getline( ifs, naming );

  //Ask for the name of the bmp file
  printf ("The file opened is ------> ");
  cout << cc;
  printf ("\n\n");

  if ( bmp.LoadBMP (cc.c_str()))
      return 0;

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
                 case 3:
                     cout << "512x512\n\n";
                     width_tile = 512;
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

            total_cut = width_tile + minus_sep;

            //new temp memory for the temp tile
            bmp_temp.Raster = new char*[total_cut];
            for( i = 0; i <= total_cut; i++ )
		          bmp_temp.Raster[i] = new char[total_cut];

            nver = (bmp.Width / total_cut);

            cout << "Name of the FILE in which is gonna be saved -> ";
            cout << string1;
            cout << endl;

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

                     nhor--;
                     nhor_aux++;



                     x_aux++;

                   switch( nhor_aux - 1 )
                   {
                           case 0:
                               switch( nver_aux )
                               {
                                   case 0:
                                       string1[7] = naming[0];
                                       cout << "TILE " << naming[0] << " ---> DONE" << endl;
                                   break;
                                   case 1:
                                       string1[7] = naming[2];
                                       cout << "TILE " << naming[2] << " ---> DONE" << endl;
                                   break;
                                   case 2:
                                       string1[7] = naming[4];
                                       cout << "TILE " << naming[4] << " ---> DONE" << endl;
                                   break;
                                   case 3:
                                       string1[7] = naming[6];
                                       cout << "TILE " << naming[6] << " ---> DONE" << endl;
                                   break;
                                   case 4:
                                       string1[7] = naming[8];
                                       cout << "TILE " << naming[8] << " ---> DONE" << endl;
                                   break;
                                   case 5:
                                       string1[7] = naming[10];
                                       cout << "TILE " << naming[10] << " ---> DONE" << endl;
                                   break;
                                   case 6:
                                       string1[7] = naming[12];
                                       cout << "TILE " << naming[12] << " ---> DONE" << endl;
                                   break;
                                   case 7:
                                       string1[7] = naming[14];
                                       cout << "TILE " << naming[14] << " ---> DONE" << endl;
                                   break;
                                   case 8:
                                       string1[7] = naming[16];
                                       cout << "TILE " << naming[16] << " ---> DONE" << endl;
                                   break;
                                   case 9:
                                       string1[7] = naming[18];
                                       cout << "TILE " << naming[18] << " ---> DONE" << endl;
                                   break;
                                   case 10:
                                       string1[7] = naming[20];
                                       cout << "TILE " << naming[20] << " ---> DONE" << endl;
                                   break;
                                   case 11:
                                       string1[7] = naming[22];
                                       cout << "TILE " << naming[22] << " ---> DONE" << endl;
                                   break;
                                   case 12:
                                       string1[7] = naming[24];
                                       cout << "TILE " << naming[24] << " ---> DONE" << endl;
                                   break;
                                   case 13:
                                       string1[7] = naming[26];
                                       cout << "TILE " << naming[26] << " ---> DONE" << endl;
                                   break;
                                   case 14:
                                       string1[7] = naming[28];
                                       cout << "TILE " << naming[28] << " ---> DONE" << endl;
                                   break;
                                   case 15:
                                       string1[7] = naming[30];
                                       cout << "TILE " << naming[30] << " ---> DONE" << endl;
                                   break;

                                   default:
                                   break;
                               }
                           break;

                           default:
                           break;
                   }

                   if(nfiles_aux>0){
                     nfiles_aux--;
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
            }

                nhor_aux = 0;
                nver--;
                nver_aux++;
            }
          cout << "Cutting of the set done, " << x_aux << " files should be done" << endl;
          system("PAUSE");

          pr << "ENDSET" << endl << endl;

 nfiles--;
 }
 //end while
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


// *************************************************************
//                  11/23/2002 Tomas GPozuelo
//                    <Com_GP@hotmail.com>
// *************************************************************

#include <windows.h>
#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>

int const tilewidth = 256;
typedef RGBQUAD Matriz24bpp[tilewidth][tilewidth];
int const transwidth = (tilewidth/4);

typedef RGBQUAD Paleta[256];

class CRaster {
	public:
		int Width,Height;		// Dimensions
		int BPP;				// Bits Per Pixel.
		char ** Raster;         // Bits of the Image. 8bpp
        RGBQUAD ** Raster24;    // Bits of the Image. 24bpp
		RGBQUAD * Palette;		// RGB Palette for the image.
		int BytesPerRow;		// Row Width (in bytes).
		BITMAPINFO * pbmi;		// BITMAPINFO structure

		// Member functions (defined later):
		void LoadBMP (char * szFile, Paleta p );
  		void SaveBMP (char * szFile, CRaster bmp, int temp_i );
};



// **********
// CRaster::LoadBMPFile (FileName);
//   - loads a BMP file into a CRaster object
//   * supports non-RLE-compressed files of 1, 2, 4, 8 & 24 bits-per-pixel
void CRaster::LoadBMP (char * szFile, Paleta p )
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
	if (BPP==24)
        pbmi=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
	else
	{   //aqui abajo hay un error para cargar multiples imagenes
        pbmi=(BITMAPINFO*)new char [sizeof(BITMAPINFO)];
//        bmpfile.read (pbmi,sizeof (BITMAPINFO));
        int t;
        for ( t = 0; t < 256; t++ )
		    bmpfile.read ((char*)&p[t], sizeof(char)*4);

        cout << "Loading PALETTE ..." << endl;
	}
	pbmi->bmiHeader=bmih;
    cout << "1" << endl;
	// Load Raster
	bmpfile.seekg (bmfh.bfOffBits,ios::beg);
    cout << BytesPerRow << " 2" << endl;
//	Raster= new char[BytesPerRow*Height];

    int i;

	//se reserva memoria para las filas
    Raster = new char*[BytesPerRow];
    cout << "3" << endl;
	//se reserva memoria para las columnas
	for( i = 0; i <= Width; i++ )
		Raster[i] = new char[Height];
    cout << "Reserving Memory ... " << endl;

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
}

void SaveBMP (char * szFile, CRaster bmp, int temp_i )
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

//    cout << bmp.Palette << " " << char(bmp.Palette) << endl;
//    system("PAUSE");
//	bmpfile.write ((char*)&bmp.Palette,256*sizeof (RGBQUAD) * (1<<bmp.BPP));

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

  CRaster bmp1, bmp2;
  int width_tile, minus_sep, nhor, nver, acum1=0, acum2=0, nver_aux, nhor_aux, i_aux,j_aux;

  //CONTADOR DE TILES
  int x_aux = 0;

  char cc1 [256];
  char cc2 [256];
  char string1 [16];

  Paleta p1, p2;

  //Ask for the name of the 1 bmp file
  printf ("Name of the first file you want to open (EX: Htile**.bmp)\n");
  gets (cc1);
  printf ("\n\n");
  bmp1.LoadBMP (cc1, p1);

  //Ask for the name of the 2 bmp file
  printf ("Name of the second file you want to open (EX: Htile**.bmp)\n");
  gets (cc2);
  printf ("\n\n");
  bmp2.LoadBMP (cc2, p2);

  //The width of the tile
  width_tile = tilewidth;
  //Separation between tiles
  minus_sep = 2;


  int i,j; //VARIABLES USADAS PARA LOS BUCLES FOR
  int numpal;//NUMERO DE LA PALETA
  tagRGBQUAD rgb_temp;
  int unitifs = (transwidth/8);
/*
  Matriz24bpp htile0;
  //     ...


  Matriz24bpp htile1;

  for( i = 0; i < tilewidth; i++){ //PARA COPIAR LOS COLORES DEL TILE EN LA MATRIZ
      for( j = 0; j < tilewidth; j++){
          numpal = bmp1.Raster[i][j];
          htile1[i][j] = bmp1.Palette[numpal];
      }
  }
  for( i = 0; i < transwidth; i++){ //PARA HACER LA TRANSICION
      for(j = 0; j < (transwidth-i); j++ ){
          if(( 0 <= i < (2*unitifs) )||( 0 <= j < (2*unitifs) )) {  //CONTENIDO ENTRE [0,7] 100%
              numpal = bmp2.Raster[i][j];
              htile1[i][j] = bmp2.Palette[numpal];
          }

          if(( (2*unitifs) <= i < (4*unitifs) )||( (2*unitifs) <= j < (4*unitifs) )) {  //CONTENIDO ENTRE [8,15] 90%
              numpal = bmp2.Raster[i][j];
              rgb_temp = bmp2.Palette[numpal];
              htile1[i][j].rgbRed = ( 0.9 * rgb_temp.rgbRed + 0.1 * htile1[i][j].rgbRed );
              htile1[i][j].rgbGreen = ( 0.9 * rgb_temp.rgbGreen + 0.1 * htile1[i][j].rgbGreen );
              htile1[i][j].rgbBlue = ( 0.9 * rgb_temp.rgbBlue + 0.1 * htile1[i][j].rgbBlue );
          }

          if(( (4*unitifs) <= i < (5*unitifs) )||( (4*unitifs) <= j < (5*unitifs) )) {  //CONTENIDO ENTRE [16,19] 65%
              numpal = bmp2.Raster[i][j];
              rgb_temp = bmp2.Palette[numpal];
              htile1[i][j].rgbRed = ( 0.65 * rgb_temp.rgbRed + 0.35 * htile1[i][j].rgbRed );
              htile1[i][j].rgbGreen = ( 0.65 * rgb_temp.rgbGreen + 0.35 * htile1[i][j].rgbGreen );
              htile1[i][j].rgbBlue = ( 0.65 * rgb_temp.rgbBlue + 0.35 * htile1[i][j].rgbBlue );
          }

          if(( (5*unitifs) <= i < (6*unitifs) )||( (5*unitifs) <= j < (6*unitifs) )) {  //CONTENIDO ENTRE [20,23] 50%
              numpal = bmp2.Raster[i][j];
              rgb_temp = bmp2.Palette[numpal];
              htile1[i][j].rgbRed = ( 0.5 * rgb_temp.rgbRed + 0.5 * htile1[i][j].rgbRed );
              htile1[i][j].rgbGreen = ( 0.5 * rgb_temp.rgbGreen + 0.5 * htile1[i][j].rgbGreen );
              htile1[i][j].rgbBlue = ( 0.5 * rgb_temp.rgbBlue + 0.5 * htile1[i][j].rgbBlue );
          }

          if(( (6*unitifs) <= i < (7*unitifs) )||( (6*unitifs) <= j < (7*unitifs) )) {  //CONTENIDO ENTRE [24,27] 35%
              numpal = bmp2.Raster[i][j];
              rgb_temp = bmp2.Palette[numpal];
              htile1[i][j].rgbRed = ( 0.35 * rgb_temp.rgbRed + 0.65 * htile1[i][j].rgbRed );
              htile1[i][j].rgbGreen = ( 0.35 * rgb_temp.rgbGreen + 0.65 * htile1[i][j].rgbGreen );
              htile1[i][j].rgbBlue = ( 0.35 * rgb_temp.rgbBlue + 0.65 * htile1[i][j].rgbBlue );
          }

          if(( (7*unitifs) <= i < (8*unitifs) )||( (7*unitifs) <= j < (8*unitifs) )) {  //CONTENIDO ENTRE [28,31] 10%
              numpal = bmp2.Raster[i][j];
              rgb_temp = bmp2.Palette[numpal];
              htile1[i][j].rgbRed = ( 0.1 * rgb_temp.rgbRed + 0.9 * htile1[i][j].rgbRed );
              htile1[i][j].rgbGreen = ( 0.1 * rgb_temp.rgbGreen + 0.9 * htile1[i][j].rgbGreen );
              htile1[i][j].rgbBlue = ( 0.1 * rgb_temp.rgbBlue + 0.9 * htile1[i][j].rgbBlue );
          }
      }
  }
  system("PAUSE");
  */
  //A PARTIR DE AQUI HACIA ABAJO YA SE VERA
  //ASIGNACIONES DE TODOS LOS CAMPOS .....CAMBIAR

  CRaster bmp_temp;  //El fichero bmp donde se van a guardar las transiciones en 24bpp

  bmp_temp.Width = ((tilewidth + 2)*5);
  bmp_temp.Height = ((tilewidth + 2)*4);
  bmp_temp.BPP = 24;
//  bmp_temp.Palette = bmp.Palette; NO SEGURO DE QUE HACER TODAVIA
  bmp_temp.pbmi = bmp1.pbmi;
  bmp_temp.pbmi->bmiHeader.biHeight = ((tilewidth + 2)*5);
  bmp_temp.pbmi->bmiHeader.biWidth = ((tilewidth + 2)*4);
  bmp_temp.pbmi->bmiHeader.biBitCount = 24;

  //RESERVA MEMORIA PARA LA IMAGEN BMP GRANDE
  bmp_temp.Raster24 = new RGBQUAD*[bmp_temp.Width];
  for( i = 0; i <= bmp_temp.Width; i++ )
      bmp_temp.Raster24[i] = new RGBQUAD[bmp_temp.Height];



  nver = ( bmp_temp.Width / tilewidth);
            cout << "Name of the FILE (ex: Hname00**.bmp, NO MORE THAN 5 CHARACTERS) to save\n";
            gets(string1);
//            string1 += "Hname**";
//            string1 = string1 + ".bmp";

            int temp_i;
            cout << "CORRECTOR FOR INCLINATION, USUALLY 0 --> ";
            cin >> temp_i;


            nver_aux = 0;
            nhor_aux = 0;
            while( nver > 0)
            {
                nhor = ( bmp_temp.Height / tilewidth );
                while( nhor > 0)
                {
                     acum1 = 0;
                     i_aux = 0;
                     for ( i = (tilewidth*(acum1+nhor_aux)); i < (((acum1+nhor_aux)*tilewidth)+(width_tile)); i++ )
                     {
                         acum2 = 0;
                         j_aux = 0;
                         for ( j = (tilewidth*(acum2+nver_aux)); j < (((acum2+nver_aux)*tilewidth)+(width_tile)); j++ )
                         {
//                               bmp_temp.Raster[i_aux][j_aux] = bmp.Raster[i][j];//Aqui hay algun problema
//                               cout <<"[" << i_aux << "][" << j_aux <<"]," <<"  [" << i << "][" << j <<"]" << endl;
                               j_aux++;
                         }
//                         system("PAUSE");
                         acum2++;
                         i_aux++;
                     }
                     acum1++;
//                     nver_aux = 0;

                     cout <<"[" << nver << "][" << nhor <<"]" << endl;
                     cout <<"[" << nver_aux << "][" << nhor_aux <<"]" << endl << endl;

                     nhor--;
                     nhor_aux++;



                     x_aux++;

//Borrar                   cout << (string1[5]) << " " << char(nver_aux) << endl;
                   string1[7] = char(48 + nhor_aux - 1);
                   string1[8] = char(48 + nver_aux);

                     //GUARDAR EN FICHERO
                     SaveBMP (string1, bmp_temp, temp_i );
//                     system("PAUSE");




                }

                nhor_aux = 0;
                nver--;
                nver_aux++;
            }
          cout << "CUTTING DONE THE " << x_aux << " FILES SHOULD BE DONE" << endl;
          system("PAUSE");
//end     FIN

//
    return 0;
}

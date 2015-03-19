#include <iostream>
#include <fstream>
#include <conio.h>
//#include <stdio.h>



typedef unsigned long LONG;
typedef unsigned short WORD;
typedef unsigned long DWORD;
using namespace std;
typedef struct tagBITMAPFILEHEADER
{
uint16_t bfType; //specifies the file type
uint32_t bfSize; //specifies the size in bytes of the bitmap file
uint16_t bfReserved1; //reserved; must be 0
uint16_t bfReserved2; //reserved; must be 0
uint32_t bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

//the bftype field checks to see if you are infact loading a bmp file, and if you are the field should be 0x4D42.

//Now we need to create our bitmapinfoheader struct. This holds info about our bitmap(self explainitory).

typedef struct tagBITMAPINFOHEADER{
	uint32_t biSize; //specifies the number of bytes required by the struct/header
	uint32_t biWidth; //specifies width in pixels
	uint32_t biHeight; //species height in pixels
	uint16_t biPlanes; //specifies the number of color planes, must be 1
	uint16_t biBitCount; //specifies the number of bit per pixel
	uint32_t biCompression;//specifies the type of compression
	uint32_t biSizeImage; //size of image in bytes
	int32_t biXPelsPerMeter; //number of pixels per meter in x axis
	int32_t biYPelsPerMeter; //number of pixels per meter in y axis
	uint32_t biClrUsed; //number of colors used by th ebitmap
	uint32_t biClrImportant; //number of colors that are important
}
BITMAPINFOHEADER;

int imagesize;
//Now on to loading our Bitmap

uint8_t *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr; //our file pointer
	BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
	uint8_t *bitmapImage; //store image data
	int imageIdx=0; //image index counter
	unsigned char tempRGB; //our swap variable

	//open filename in read binary mode
	filePtr = fopen(filename,"rb");
	if (filePtr == NULL){
		cout<<"couldn't open the file";
		return NULL;
	}
	//read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

	//cout<<bitmapFileHeader.bfType;
	//cout<<bitmapFileHeader.bfOffBits;
	//verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.bfType !=0x4D42){
		cout<<bitmapFileHeader.bfType;
		cout<<"some problem with bf type";
		fclose(filePtr);
		return NULL;
	}
	//printf("Height: %d\n", bitmapInfoHeader->biHeight);
	//read the bitmap info header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);
	//int i;
	//for(i=0;i<sizeof(BITMAPINFOHEADER);i++)
	//{
	//	cout<<&bitmapInfoHeader[i]<<"    ";
	//}
	//cout<<"\n i="<<i <<"\n";

	//printf("Height: %d\n", bitmapInfoHeader->biHeight);
	//cout<<"bi planes"<<bitmapInfoHeader->biPlanes<<"  ";
	//fseek(filePtr,0x16, SEEK_SET);
	//cout<<"bi width"<<bitmapInfoHeader->biWidth<<"  ";
	//cout<<"bi size"<<bitmapInfoHeader->biSize<<"  ";
	//printf("biplanes  %x  ", bitmapInfoHeader->biPlanes);
	//cout<<"bi bitcount"<<bitmapInfoHeader->biBitCount<<"  ";
	//cout<<"bi compression"<<bitmapInfoHeader->biCompression<<"  ";
	//move file point to the begging of bitmap data
	//rewind(filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//allocate enough memory for the bitmap image data
	bitmapImage = (uint8_t*)malloc(bitmapInfoHeader->biSizeImage);

	//verify memory allocation
	if (!bitmapImage){
		free(bitmapImage);
		cout<<"couldnt create memory location";
		fclose(filePtr);
		return NULL;
	}

	//read in the bitmap image data
	fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

	//make sure bitmap image data was read
	if (bitmapImage == NULL){
		cout<<"bitmap image not read";
		fclose(filePtr);
		return NULL;
	}

	//swap the r and b values to get RGB (bitmap is BGR)
	imagesize = bitmapInfoHeader->biSizeImage;
	//cout<<bitmapInfoHeader->biSizeImage;
	for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3){
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	//close file and return bitmap iamge data
	fclose(filePtr);
		return bitmapImage;
	}


//Now to make use of all of this:

int main(){
	BITMAPINFOHEADER bitmapInfoHeader;
	uint8_t *bitmapData;
	fflush(stdin);
	bitmapData = LoadBitmapFile("d:\\lena.bmp",&bitmapInfoHeader);
	//now do what you want with it, later on i will show you how to display it in a normal window
	//cout<<"int"<<sizeof(int)<< "short"<<sizeof(short)<<"long"<<sizeof(long);
	for (int imageIdx = 0;imageIdx < 1000 ;imageIdx++){
		cout<<bitmapData[imageIdx];
	}
	getch();
	return 0;
}
#include "stdafx.h"
#include "simple_media_raw.h"
/**
*	Split Y, U, V planes in YUV420P file. 
*
*
*
**********/

void simple_yuv420p_split(const char* url,int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_420_y.y", "wb+");
	FILE *fp2 = fopen("output_420_u.y", "wb+");
	FILE *fp3 = fopen("output_420_v.y", "wb+");

	unsigned char* pic = (unsigned char*)malloc(w*h*3/2);

	for (int i = 0; i < num; i++)
	{
		fread(pic, 1, w*h *3/2, fp);
		//Y
		fwrite(pic, 1, w*h, fp1);
		//U
		fwrite(pic + w*h, 1, w*h/4, fp2);
		//V
		fwrite(pic + w*h*5/4, 1, w*h/4, fp3);
	}
	
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

/**
*	Split Y, U, V planes in YUV422P file.
*
*
*
**********/
void simple_yuv422p_split(const char* url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_422_y.y", "wb+");
	FILE *fp2 = fopen("output_422_u.y", "wb+");
	FILE *fp3 = fopen("output_422_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w*h * 2);

	for (int i = 0; i<num; i++){
		fread(pic, 1, w*h*2, fp);
		//Y  
		fwrite(pic, 1, w*h, fp1);
		//U  
		fwrite(pic + w*h, 1, w*h*1/2, fp2);
		//V  
		fwrite(pic + w*h*3/2, 1, w*h*1/2, fp3);
	}

	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

/**
*	Split Y, U, V planes in YUV444P file.
*
*
*
**********/
void simple_yuv444p_split(const char* url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_444_y.y", "wb+");
	FILE *fp2 = fopen("output_444_u.y", "wb+");
	FILE *fp3 = fopen("output_444_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w*h * 3);

	for (int i = 0; i<num; i++){
		fread(pic, 1, w*h * 3, fp);
		//Y  
		fwrite(pic, 1, w*h, fp1);
		//U  
		fwrite(pic + w*h, 1, w*h, fp2);
		//V  
		fwrite(pic + w*h * 2, 1, w*h, fp3);
	}

	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}


/**
*	Convert YUV420P file to gray picture
*
*
*
**********/
void simple_yuv420_gray(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_420_gray.yuv", "wb+");

	unsigned char* pic = (unsigned char*)malloc(w*h * 3 / 2);
	for (int i = 0; i < num; i++)
	{
		fread(pic, 1, w*h * 3 / 2, fp);
		//Gray  
		memset(pic + w*h, 128, w*h / 2);
		fwrite(pic, 1, w*h * 3 / 2, fp1);
	}

	free(pic);
	fclose(fp);
	fclose(fp1);

}

/**
*	Halve Y value of YUV420P file 
*
*
*
**********/
void simple_yuv420_halve(char *url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_420_halve.yuv", "wb+");

	unsigned char* pic = (unsigned char*)malloc(w*h * 3 / 2);
	for (int i = 0; i < num; i++)
	{
		fread(pic, 1, w*h * 3 / 2, fp);
		//halve Y
		for (int j = 0; j < w*h; j++)
		{
			unsigned char t = pic[j];
			pic[j] = t / 2;
		}
		fwrite(pic, 1, w*h * 3 / 2, fp1);
	}

	free(pic);
	fclose(fp);
	fclose(fp1);
}

/**
*	Add a border for YUV420P file
*
*
*
**********/
void simple_yuv420_border(char *url, int w, int h, int border, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_420_border.yuv", "wb+");

	unsigned char* pic = (unsigned char*)malloc(w*h * 3 / 2);
	for (int i = 0; i < num; i++)
	{
		fread(pic, 1, w*h * 3 / 2, fp);
		//border Y
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				if (k < border || k > w - border || j < border || j > h - border)
				{
					pic[j*h + k] = 255;
				}
			}			
		}
		fwrite(pic, 1, w*h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
}

/**
*	Generate YUV420P gray scale bar.
*
*
*
**********/
void simple_yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, char *url_out)
{
	FILE *fp = NULL;
	unsigned char *data_y = NULL;
	unsigned char *data_u = NULL;
	unsigned char *data_v = NULL;


	fp = fopen(url_out, "wb+");
	if (fp == NULL)
	{
		printf("can't open file\n");
	}
	float inc = ((float)(ymax - ymin)) / ((float)(barnum - 1));
	int per = width / barnum;
	data_y = (unsigned char*)malloc(width*height);
	data_u = (unsigned char*)malloc(width*height/4);
	data_v = (unsigned char*)malloc(width*height/4);

	//	Y
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int k = j / per;
			data_y[i*width + j] = ymin + (char)k*inc;
		}
	}


	for (int i = 0; i < height/2; i++)
	{
		for (int j = 0; j < width/2; j++)
		{
			data_u[i*width/ 2 + j] = 128;
		}
	}

	for (int i = 0; i < height/2; i++)
	{
		for (int j = 0; j < width/2; j++)
		{
			data_v[i*width / 2 + j] = 128;
		}
	}

	fwrite(data_y, width*height, 1, fp);
	fwrite(data_u, width*height/4, 1, fp);
	fwrite(data_v, width*height/4, 1, fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
}

/**
*	calculate two YUV420p pictures's psrn.
*
*
*
**********/
void simple_yuv420_psnr(const char* url1, const char* url2, int h, int w, int num)
{

	FILE *fp1 = fopen(url1, "rb+");
	FILE *fp2 = fopen(url2, "rb+");
	unsigned char *pic1 = (unsigned char *)malloc(w*h);
	unsigned char *pic2 = (unsigned char *)malloc(w*h);

	fread(pic1, w*h, 1, fp1);
	fread(pic2, w*h, 1, fp2);

	double mse_sum = 0, mse = 0, psnr = 0;
	for (int i = 0; i < h*w; i++)
	{
		mse_sum += pow(double(pic1[i] - pic2[i]), 2);
	}
	mse = mse_sum / (h*w);
	psnr = 10 * log10(255.0*255.0 / mse);
	printf("%5.3f\n", psnr);

	fseek(fp1, w*h / 2, SEEK_CUR);
	fseek(fp2, w*h / 2, SEEK_CUR);

	free(pic1);
	free(pic2);
	fclose(fp1);
	fclose(fp2);

}

/**
*	split r g b panels from rgb24 picture
*
*
*
**********/
void simple_rgb24_split(const char* url, int w, int h, int num)
{
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("output_r.y", "wb+");
	FILE *fp2 = fopen("output_g.y", "wb+");
	FILE *fp3 = fopen("output_b.y", "wb+");

	unsigned char* pic = (unsigned char*)malloc(w*h*3);

	for (int i = 0; i < num; i++)
	{
		fread(pic, w*h * 3, 1, fp);
		for (int j = 0; j < w*h*3; j+=3)
		{
			fwrite(pic+j, 1, 1, fp1);
			fwrite(pic + j + 1, 1, 1, fp2);
			fwrite(pic + j + 2, 1, 1, fp3);
		}
	}

	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

/**
*	Convert RGB24 file to BMP file
*
*
*
**********/
void simple_rgb24_to_bmp(const char *rgb24path, int width, int height, const char *bmppath)
{
	typedef struct
	{
		long imagesize;
		long black;
		long startPosition;
	}BmpHead;

	typedef struct
	{
		long  size;
		long  width;
		long  height;
		unsigned short  colorPlane;
		unsigned short  bitColor;
		long  zipFormat;
		long  realSize;
		long  xPels;
		long  yPels;
		long  colorUse;
		long  colorImportant;
	}InfoHead;

	char bfType[2] = { 'B', 'M' };
	BmpHead bmpHead = { 0 };
	InfoHead infoHead = { 0 };

	int head_size = sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
	bmpHead.imagesize = head_size + width*height * 3;
	bmpHead.startPosition = head_size;

	unsigned char* rgb_buffer = (unsigned char*)malloc(width*height * 3);
	FILE *fpRgb = NULL, *fpBmp = NULL;
	fpRgb = fopen(rgb24path, "rb");
	fpBmp = fopen(bmppath, "wb");
	fread(rgb_buffer,width*height*3,1,fpRgb);

	bmpHead.imagesize = 3 * width*height + head_size;
	bmpHead.startPosition = head_size;

	infoHead.size = sizeof(InfoHead);
	infoHead.width = width;
	//BMP storage pixel data in opposite direction of Y-axis (from bottom to top).  
	infoHead.height = -height;
	infoHead.colorPlane = 1;
	infoHead.bitColor = 24;
	infoHead.realSize = 3 * width*height;

	fwrite(bfType, sizeof(bfType),1,fpBmp);
	fwrite(&bmpHead,sizeof(BmpHead),1,fpBmp);
	fwrite(&infoHead,sizeof(InfoHead),1,fpBmp);

	//BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2  
    //It saves pixel data in Little Endian  
    //So we change 'R' and 'B'  
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char tmp = rgb_buffer[(i*width + j)*3 + 2];
			rgb_buffer[(i*width + j)*3 + 2] = rgb_buffer[(i*width + j)*3];
			rgb_buffer[(i*width + j)*3] = tmp;
		}
	}

	fwrite(rgb_buffer, width*height * 3, 1, fpBmp);
	fclose(fpRgb);
	fclose(fpBmp);
	free(rgb_buffer);
	printf("Finish generate %s!\n", bmppath);
}


unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char  max_val){
	if (x>max_val){
		return max_val;
	}
	else if (x<min_val){
		return min_val;
	}
	else{
		return x;
	}
}
/**
*	Convert RGB24 file to YVU420 file
*
*
*
**********/
void simple_rgb24_to_yuv420(const char *rgb24path, int width, int height, const char *yuv420path)
{
	FILE* fprgb = fopen(rgb24path,"rb+");
	FILE* fpyuv = fopen(yuv420path, "wb+");
	unsigned char* rgb_buffer = (unsigned char*)malloc(width*height*3);
	unsigned char* y_buffer = (unsigned char*)malloc(width*height);
	unsigned char* u_buffer = (unsigned char*)malloc(width*height/4);
	unsigned char* v_buffer = (unsigned char*)malloc(width*height/4);
	unsigned char* u_tmp = u_buffer;
	unsigned char* v_tmp = v_buffer;

	unsigned char r = 0, g = 0, b = 0;
	unsigned char y = 0, u = 0, v = 0;
	fread(rgb_buffer, width*height * 3, 1, fprgb);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			r = rgb_buffer[(i*width + j) * 3];
			g = rgb_buffer[(i*width + j) * 3 + 1];
			b = rgb_buffer[(i*width + j) * 3 + 2];


			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
			v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

			y_buffer[i*width + j] = clip_value(y, 0, 255);
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
					*(u_buffer++) = clip_value(u, 0, 255);
			}
			else{
				if (j % 2 == 0){
					*(v_buffer++) = clip_value(v, 0, 255);
				}
			}

		}
	}
	
	fwrite(y_buffer,width*height,1,fpyuv);
	fwrite(u_tmp, width*height / 4, 1, fpyuv);
	fwrite(v_tmp, width*height / 4, 1, fpyuv);

	free(rgb_buffer);
	free(y_buffer);
	free(u_tmp);
	free(v_tmp);
	fclose(fprgb);
	fclose(fpyuv);
}

/**
*	Generate RGB24 colorbar.
*
*
*
**********/
void simple_rgb24_colorbar(int width, int height, char *url_out)
{
	unsigned char *data = NULL;
	int barwidth;
	char filename[100] = { 0 };
	FILE *fp = NULL;
	int i = 0, j = 0;

	data = (unsigned char *)malloc(width*height * 3);
	barwidth = width / 8;

	if ((fp = fopen(url_out, "wb+")) == NULL){
		printf("Error: Cannot create file!");
		return;
	}

	for (j = 0; j<height; j++){
		for (i = 0; i<width; i++){
			int barnum = i / barwidth;
			switch (barnum){
			case 0:{
					   data[(j*width + i) * 3 + 0] = 255;
					   data[(j*width + i) * 3 + 1] = 255;
					   data[(j*width + i) * 3 + 2] = 255;
					   break;
			}
			case 1:{
					   data[(j*width + i) * 3 + 0] = 255;
					   data[(j*width + i) * 3 + 1] = 255;
					   data[(j*width + i) * 3 + 2] = 0;
					   break;
			}
			case 2:{
					   data[(j*width + i) * 3 + 0] = 0;
					   data[(j*width + i) * 3 + 1] = 255;
					   data[(j*width + i) * 3 + 2] = 255;
					   break;
			}
			case 3:{
					   data[(j*width + i) * 3 + 0] = 0;
					   data[(j*width + i) * 3 + 1] = 255;
					   data[(j*width + i) * 3 + 2] = 0;
					   break;
			}
			case 4:{
					   data[(j*width + i) * 3 + 0] = 255;
					   data[(j*width + i) * 3 + 1] = 0;
					   data[(j*width + i) * 3 + 2] = 255;
					   break;
			}
			case 5:{
					   data[(j*width + i) * 3 + 0] = 255;
					   data[(j*width + i) * 3 + 1] = 0;
					   data[(j*width + i) * 3 + 2] = 0;
					   break;
			}
			case 6:{
					   data[(j*width + i) * 3 + 0] = 0;
					   data[(j*width + i) * 3 + 1] = 0;
					   data[(j*width + i) * 3 + 2] = 255;

					   break;
			}
			case 7:{
					   data[(j*width + i) * 3 + 0] = 0;
					   data[(j*width + i) * 3 + 1] = 0;
					   data[(j*width + i) * 3 + 2] = 0;
					   break;
			}
			}

		}
	}
	fwrite(data, width*height * 3, 1, fp);
	fclose(fp);
	free(data);
}
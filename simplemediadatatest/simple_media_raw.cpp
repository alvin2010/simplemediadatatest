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
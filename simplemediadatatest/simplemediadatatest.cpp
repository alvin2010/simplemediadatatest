// simplemediadatatest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "simple_media_raw.h"

int _tmain(int argc, _TCHAR* argv[])
{

	// split
	simple_yuv420p_split("lena_256x256_yuv420p.yuv", 256, 256, 1);
	simple_yuv422p_split("lena_256x256_yuv422p.yuv", 256, 256, 1);
	simple_yuv444p_split("lena_256x256_yuv444p.yuv", 256, 256, 1);

	// gray
	simple_yuv420_gray("lena_256x256_yuv420p.yuv", 256, 256, 1);

	//halve Y
	simple_yuv420_halve("lena_256x256_yuv420p.yuv", 256, 256, 1);
	simple_yuv420_border("lena_256x256_yuv420p.yuv", 256, 256, 20, 1);
	
	//	graybar
	simple_yuv420_graybar(640, 360, 0, 255, 10, "graybar_640x360.yuv");

	//	psnr
	simple_yuv420_psnr("lena_256x256_yuv420p.yuv", "lena_distort_256x256_yuv420p.yuv", 256, 256, 1);

	
	return 0;
}


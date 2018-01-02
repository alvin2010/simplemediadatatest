// simplemediadatatest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "simple_media_raw.h"
#include "simple_h264.h"
#include "simple_aac.h"
#include "simple_flv.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/*
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
	simple_yuv420_graybar(640, 360, 0, 255, 10, "output_graybar_640x360.yuv");

	//	psnr
	simple_yuv420_psnr("lena_256x256_yuv420p.yuv", "lena_distort_256x256_yuv420p.yuv", 256, 256, 1);

	// split
	simple_rgb24_split("cie1931_500x500.rgb", 500, 500, 1);
	simple_rgb24_to_bmp("lena_256x256_rgb24.rgb", 256, 256, "output_lena.bmp");
	simple_rgb24_to_yuv420("lena_256x256_rgb24.rgb", 256, 256, "output_lena.yuv");
	simple_rgb24_colorbar(640, 360, "output_colorbar.rgb");

	// spit
	simple_pcm16le_split("NocturneNo2inEflat_44.1k_s16le.pcm");
	simple_pcm16le_halfvolumeleft("NocturneNo2inEflat_44.1k_s16le.pcm");
	simple_pcm16le_doublespeed("NocturneNo2inEflat_44.1k_s16le.pcm");
	simple_pcm16le_to_pcm8("NocturneNo2inEflat_44.1k_s16le.pcm");
	simple_pcm16le_cut_singlechannel("drum.pcm", 2360, 120);
	simple_pcm16le_to_wav("NocturneNo2inEflat_44.1k_s16le.pcm", 2, 44100, "output_nocturne.wav");

	//	h264
	simple_parse_h264("sintel.h264");
	simple_parse_aac("nocturne.aac");
	*/

	simple_parse_flv("cuc_ieschool.flv");
	
	return 0;
}


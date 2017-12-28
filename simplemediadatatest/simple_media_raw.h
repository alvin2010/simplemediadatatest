
#ifndef SIMPLE_MEDIA_RAW_H_
#define SIMPLE_MEDIA_RAW_H_

void simple_yuv420p_split(const char* url, int w, int h, int num);

void simple_yuv422p_split(const char* url, int w, int h, int num);

void simple_yuv444p_split(const char* url, int w, int h, int num);

void simple_yuv420_gray(char *url, int w, int h, int num);

void simple_yuv420_halve(char *url, int w, int h, int num);

void simple_yuv420_border(char *url, int w, int h, int border, int num);

void simple_yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, char *url_out);

void simple_yuv420_psnr(const char* url1, const char* url2, int h, int w, int num);

void simple_rgb24_split(const char* url, int w, int h, int num);

void simple_rgb24_to_bmp(const char *rgb24path, int width, int height, const char *bmppath);

void simple_rgb24_to_yuv420(const char *rgb24path, int width, int height, const char *yuv420path);

void simple_rgb24_colorbar(int width, int height, char *url_out);

#endif
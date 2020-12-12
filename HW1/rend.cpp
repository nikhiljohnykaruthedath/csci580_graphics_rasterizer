#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"

GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;
	framebuffer = (char *) malloc(xres * yres * 64 * sizeof(char));
	pixelbuffer = static_cast<GzPixel*>(malloc(xres * yres * 64 * sizeof(struct GzPixel*)));
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	if (framebuffer != NULL) {
		delete framebuffer;
	}
	if (pixelbuffer != NULL) {
		delete pixelbuffer;
	}
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			int index = ARRAY(i, j);
			(pixelbuffer + index)->red = 1000;
			(pixelbuffer + index)->green = 1000;
			(pixelbuffer + index)->blue = 1000;
			(pixelbuffer + index)->alpha = 1;
			(pixelbuffer + index)->z = 0;
		}
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	r = (r > 4095) ? 4095 : r;
	g = (g > 4095) ? 4095 : g;
	b = (b > 4095) ? 4095 : b;

	i = (i < 0) ? 0 : i;
	j = (j < 0) ? 0 : j;
	i = (i >= yres) ? yres - 1 : i;
	j = (j >= xres) ? xres - 1 : j;

	int index = ARRAY(i, j);
	(pixelbuffer + index)->red = r;
	(pixelbuffer + index)->green = g;
	(pixelbuffer + index)->blue = b;
	(pixelbuffer + index)->alpha = a;
	(pixelbuffer + index)->z = z;
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	int index = ARRAY(i, j);
	r = &(pixelbuffer + index)->red;
	g = &(pixelbuffer + index)->green;
	b = &(pixelbuffer + index)->blue;
	a = &(pixelbuffer + index)->alpha;
	z = &(pixelbuffer + index)->z;
	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			int index = ARRAY(i, j);

			fprintf(outfile, "%c%c%c", 
				(pixelbuffer + index)->red >> 4, 
				(pixelbuffer + index)->green >> 4,
				(pixelbuffer + index)->blue >> 4);

		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	int k = 0;
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			int index = ARRAY(i, j);
			framebuffer[k++] = (pixelbuffer + index)->blue >> 4;
			framebuffer[k++] = (pixelbuffer + index)->green >> 4;
			framebuffer[k++] = (pixelbuffer + index)->red >> 4;
		}
	}
	char* test = framebuffer;

	return GZ_SUCCESS;
}
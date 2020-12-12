/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image = NULL;
int xs, ys;
int reset = 1;

#define	ARRAY(x,y)	(x + (y * xs))

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
	unsigned char		pixel[3];
	unsigned char     dummy;
	char  		foo[8];
	int   		i, j;
	FILE			*fd;

	if (reset) {          /* open and load texture file */
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	/* determine texture cell corner values and perform bilinear interpolation */
	/* set color to interpolated GzColor value and return */

	// Clamp u and v between 0 and 1
	if (u < 0) {
		u = 0;
	}
	if (u > 1) {
		u = 1;
	}

	if (v < 0) {
		v = 0;
	}
	if (v > 1) {
		v = 1;
	}

	u = u * (xs - 1);
	v = v * (ys - 1);

	// Perform bilinear interpolation
	int A, B, C, D;
	float s, t;
	A = floor(u);
	B = ceil(u);
	C = floor(v);
	D = ceil(v);
	s = u - A;
	t = v - C;

	color[RED] = ((s * t * image[ARRAY(B, D)][RED])
		+ ((1 - s) * (t)* image[ARRAY(A, D)][RED])
		+ ((s)* (1 - t) * image[ARRAY(B, C)][RED])
		+ ((1 - s) * (1 - t) * image[ARRAY(A, C)][RED]));


	color[GREEN] = ((s * t * image[ARRAY(B, D)][GREEN])
		+ ((1 - s) * (t)* image[ARRAY(A, D)][GREEN])
		+ ((s)* (1 - t) * image[ARRAY(B, C)][GREEN])
		+ ((1 - s) * (1 - t) * image[ARRAY(A, C)][GREEN]));

	color[BLUE] = ((s * t * image[ARRAY(B, D)][BLUE])
		+ ((1 - s) * (t)* image[ARRAY(A, D)][BLUE])
		+ ((s)* (1 - t) * image[ARRAY(B, C)][BLUE])
		+ ((1 - s) * (1 - t) * image[ARRAY(A, C)][BLUE]));
  
	return GZ_SUCCESS;
}

class ComplexNumber

{

public:

	float r;
	float i;
	ComplexNumber() {};
	~ComplexNumber() {};
};

int ptex_fun(float u, float v, GzColor color) {
	// Fractal texture: Julia Set
	int N = 150;
	ComplexNumber x;
	ComplexNumber c;

	// Setting c values
	c.r = -0.624;
	c.i = 0.435;

	// X = u + vi
	x.r = 2 * u - 0.7;
	x.i = v - 0.9;

	int i;
	float clr;
	for (i = 0; i < N; i++)
	{
		//X^2 + C
		//(XrYr - XiYi)r + Cr
		float xr = x.r * x.r - x.i * x.i + c.r;
		//(XrYi + XiYr)i + Ci
		float xi = x.r * x.i + x.i * x.r + c.i;
		if ((x.r * x.r + x.i * x.i) > 4.0) {
			break;
		}
		x.r = xr;
		x.i = xi;
	}

	float lengthX = sqrt(x.r * x.r + x.i * x.i);
	if (i == N) {
		color[0] = lengthX / 2;
		color[1] = lengthX / 2;
		color[2] = lengthX / 2;
	}
	else {
		clr = ((float) i) / N;
		color[0] = clr * 10;
		color[1] = clr * 8;
		color[2] = clr * 10;
	}
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}


#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include<vector>
#include<algorithm>

/***********************************************/
/* HW1 methods: copy here the methods from HW1 */

GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = xRes;
	yres = yRes;
	framebuffer = (char *)malloc(xres * yres * 64 * sizeof(char));
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
			(pixelbuffer + index)->red = 4000;
			(pixelbuffer + index)->green = 4000;
			(pixelbuffer + index)->blue = 4000;
			(pixelbuffer + index)->alpha = 1;
			(pixelbuffer + index)->z = INT_MAX;
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
	printf("Test");
	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/
	switch (numAttributes) {
		case 1: 
			float *color = (*(GzColor *)valueList[0]);
			for (int i = 0; i < 3; i++) {
				flatcolor[i] = *color;
				color++;
			}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList) 
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Invoke the rastrizer/scanline framework
-- Return error code
*/
	float *position = (*(GzCoord *)valueList[0]);
	renderTriangle(position);
	return GZ_SUCCESS;
}

void GzRender::renderTriangle(float *position) {
	sortVertices(position);
	assignEdges();
	float xp = determineLeftRightEdge(v1, v3, v2[1]);
	if (xp > v2[0]) {
		renderLeftTriangle();
	}
	else {
		renderRightTriangle();
	}
}

void GzRender::renderLeftTriangle() {
	GzCoord spanStart, spanEnd, spanCurrent;
	float spanSlopeZ;

	float yDeltaV1V2 = findDelta(eV1V2.current[1]);
	while (eV1V2.current[1] <= floor(eV1V2.end[1])) {
		updateTopEdges(yDeltaV1V2);
		for (int i = 0; i < 3; i++) {
			spanStart[i] = eV1V2.current[i];
			spanEnd[i] = eV1V3.current[i];
			spanCurrent[i] = eV1V2.current[i];
		}
		spanSlopeZ = findSlope('s', spanStart, spanEnd);

		float deltaX = findDelta(spanCurrent[0]);
		while (spanCurrent[0] <= spanEnd[0]) {
			spanCurrent[0] = spanCurrent[0] + deltaX;
			spanCurrent[2] = spanCurrent[2] + deltaX * spanSlopeZ;
			int index = ARRAY(spanCurrent[0], spanCurrent[1]);
			int zPix = (pixelbuffer + index)->z;
			if (spanCurrent[2] < zPix) {
				GzPut(spanCurrent[0], spanCurrent[1],
					ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, spanCurrent[2]);
			}
			
		}
	}

	float yDeltaV2V3 = findDelta(eV2V3.current[1]);
	while (eV2V3.current[1] <= floor(eV2V3.end[1])) {
		updateBottomEdges(yDeltaV2V3);
		for (int i = 0; i < 3; i++) {
			spanStart[i] = eV2V3.current[i];
			spanEnd[i] = eV1V3.current[i];
			spanCurrent[i] = eV2V3.current[i];
		}
		spanSlopeZ = findSlope('s', spanStart, spanEnd);

		float deltaX = findDelta(spanCurrent[0]);
		while (spanCurrent[0] <= spanEnd[0]) {
			spanCurrent[0] = spanCurrent[0] + deltaX;
			spanCurrent[2] = spanCurrent[2] + deltaX * spanSlopeZ;
			int index = ARRAY(spanCurrent[0], spanCurrent[1]);
			int zPix = (pixelbuffer + index)->z;
			if (spanCurrent[2] < zPix) {
				GzPut(spanCurrent[0], spanCurrent[1],
					ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, spanCurrent[2]);
			}
		}
	}
}

void GzRender::renderRightTriangle() {
	GzCoord spanStart, spanEnd, spanCurrent;
	float spanSlopeZ;

	short r = 0, g = 0, b = 0, a = 0;
	int z = 0;

	float yDeltaV1V2 = findDelta(eV1V2.current[1]);
	while (eV1V2.current[1] <= floor(eV1V2.end[1])) {
		updateTopEdges(yDeltaV1V2);
		for (int i = 0; i < 3; i++) {
			spanStart[i] = eV1V3.current[i];
			spanEnd[i] = eV1V2.current[i];
			spanCurrent[i] = eV1V3.current[i];
		}
		spanSlopeZ = findSlope('s', spanStart, spanEnd);

		float deltaX = findDelta(spanCurrent[0]);
		while (spanCurrent[0] <= spanEnd[0]) {
			spanCurrent[0] = spanCurrent[0] + deltaX;
			spanCurrent[2] = spanCurrent[2] + deltaX * spanSlopeZ;
			int index = ARRAY(spanCurrent[0], spanCurrent[1]);
			int zPix = (pixelbuffer + index)->z;
			if (spanCurrent[2] < zPix) {
				GzPut(spanCurrent[0], spanCurrent[1],
					ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, spanCurrent[2]);
			}
		}
	}

	float yDeltaV2V3 = findDelta(eV2V3.current[1]);
	while (eV2V3.current[1] <= floor(eV2V3.end[1])) {
		updateBottomEdges(yDeltaV2V3);
		for (int i = 0; i < 3; i++) {
			spanStart[i] = eV1V3.current[i];
			spanEnd[i] = eV2V3.current[i];
			spanCurrent[i] = eV1V3.current[i];
		}
		spanSlopeZ = findSlope('s', spanStart, spanEnd);

		float deltaX = findDelta(spanCurrent[0]);
		while (spanCurrent[0] <= spanEnd[0]) {
			spanCurrent[0] = spanCurrent[0] + deltaX;
			spanCurrent[2] = spanCurrent[2] + deltaX * spanSlopeZ;
			int index = ARRAY(spanCurrent[0], spanCurrent[1]);
			int zPix = (pixelbuffer + index)->z;
			if (spanCurrent[2] < zPix) {
				GzPut(spanCurrent[0], spanCurrent[1],
					ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, spanCurrent[2]);
			}
		}
	}
}

bool sortHelper(const float &y1, const float &y2) {
	return (y1 < y2);
}

void GzRender::sortVertices(float *position) {
	GzCoord vert1 = { position[6], position[7], position[8] };
	GzCoord vert2 = { position[3], position[4], position[5] };
	GzCoord vert3 = { position[0], position[1], position[2] };

	std::vector<float> v;
	v.push_back(position[1]);
	v.push_back(position[4]);
	v.push_back(position[7]);

	std::sort(v.begin(), v.end(), sortHelper);

	int i = 1, j = 0;
	while (i < 8) {
		if (position[i] == v[0]) {
			v1[j++] = position[i - 1];
			v1[j++] = position[i];
			v1[j++] = position[i + 1];
			j = 0;
		}
		if (position[i] == v[1]) {
			v2[j++] = position[i - 1];
			v2[j++] = position[i];
			v2[j++] = position[i + 1];
			j = 0;
		}
		if (position[i] == v[2]) {
			v3[j++] = position[i - 1];
			v3[j++] = position[i];
			v3[j++] = position[i + 1];
			j = 0;
		}
		i += 3;
	}
}

float GzRender::determineLeftRightEdge(GzCoord v1, GzCoord v2, float y) {
	return -1 * ((v2[0] - v1[0]) * y + (v1[0] * v2[1] - v2[0] * v1[1])) / (v1[1] - v2[1]);
}

void GzRender::assignEdges() {
	for (int i = 0; i < 3; i++) {
		eV1V2.start[i] = v1[i];
		eV1V2.end[i] = v2[i];
		eV1V2.current[i] = v1[i];

		eV2V3.start[i] = v2[i];
		eV2V3.end[i] = v3[i];
		eV2V3.current[i] = v2[i];

		eV1V3.start[i] = v1[i];
		eV1V3.end[i] = v3[i];
		eV1V3.current[i] = v1[i];
	}
	eV1V2.slopeX = findSlope('x', eV1V2.start, eV1V2.end);
	eV1V2.slopeZ = findSlope('z', eV1V2.start, eV1V2.end);

	eV2V3.slopeX = findSlope('x', eV2V3.start, eV2V3.end);
	eV2V3.slopeZ = findSlope('z', eV2V3.start, eV2V3.end);

	eV1V3.slopeX = findSlope('x', eV1V3.start, eV1V3.end);
	eV1V3.slopeZ = findSlope('z', eV1V3.start, eV1V3.end);
}

void GzRender::updateTopEdges(float deltaY) {
	eV1V2.current[0] = eV1V2.current[0] + eV1V2.slopeX * deltaY;
	eV1V3.current[0] = eV1V3.current[0] + eV1V3.slopeX * deltaY;

	eV1V2.current[1] = eV1V2.current[1] + deltaY;
	eV1V3.current[1] = eV1V3.current[1] + deltaY;

	eV1V2.current[2] = eV1V2.current[2] + eV1V2.slopeZ * deltaY;
	eV1V3.current[2] = eV1V3.current[2] + eV1V3.slopeZ * deltaY;
}

void GzRender::updateBottomEdges(float deltaY) {
	eV2V3.current[0] = eV2V3.current[0] + eV2V3.slopeX * deltaY;
	eV1V3.current[0] = eV1V3.current[0] + eV1V3.slopeX * deltaY;

	eV2V3.current[1] = eV2V3.current[1] + deltaY;
	eV1V3.current[1] = eV1V3.current[1] + deltaY;

	eV2V3.current[2] = eV2V3.current[2] + eV2V3.slopeZ * deltaY;
	eV1V3.current[2] = eV1V3.current[2] + eV1V3.slopeZ * deltaY;
}

float GzRender::findSlope(char axis, GzCoord v1, GzCoord v2) {
	switch (axis) {
	case 'x':
		return (v2[0] - v1[0]) / (v2[1] - v1[1]);
		break;
	case 'z':
		return (v2[2] - v1[2]) / (v2[1] - v1[1]);
		break;
	case 's':
		return (v2[2] - v1[2]) / (v2[0] - v1[0]);
		break;
	}
}

/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include<vector>
#include<algorithm>

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
/* HW 3.1
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = 1;
	mat[1][1] = cos(degree *(PI / 180));
	mat[1][2] = -sin(degree *(PI / 180));
	mat[2][1] = sin(degree *(PI / 180));
	mat[2][2] = cos(degree *(PI / 180));

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = cos(degree *(PI / 180));
	mat[0][2] = sin(degree *(PI / 180));
	mat[1][1] = 1;
	mat[2][0] = -sin(degree *(PI / 180));
	mat[2][2] = cos(degree *(PI / 180));
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = cos(degree *(PI / 180));
	mat[0][1] = -sin(degree *(PI / 180));
	mat[1][0] = sin(degree *(PI / 180));
	mat[1][1] = cos(degree *(PI / 180));
	mat[2][2] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][3] = translate[X];
	mat[1][3] = translate[Y];
	mat[2][3] = translate[Z];
	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = scale[X];
	mat[1][1] = scale[Y];
	mat[2][2] = scale[Z];
	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;
	framebuffer = (char*)malloc(3 * sizeof(char) * xres * yres);
	pixelbuffer = static_cast<GzPixel*>(malloc(xres * yres * 64 * sizeof(struct GzPixel*)));
	// framebuffer = (char *)malloc(xres * yres * 64 * sizeof(char));
	

/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
	matlevel = 0;
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	m_camera.lookat[0] = 0;
	m_camera.lookat[1] = 0;
	m_camera.lookat[2] = 0;
	m_camera.worldup[0] = 0;
	m_camera.worldup[1] = 1;
	m_camera.worldup[2] = 0;
	m_camera.FOV = DEFAULT_FOV * (PI / 180);
	Xsp[0][0] = xRes / 2;
	Xsp[0][1] = 0;
	Xsp[0][2] = 0;
	Xsp[0][3] = xRes / 2;
	Xsp[1][0] = 0;
	Xsp[1][1] =  -(yRes / 2);
	Xsp[1][2] = 0;
	Xsp[1][3] = yRes / 2;
	Xsp[2][0] = 0;
	Xsp[2][1] = 0;
	Xsp[2][2] = INT_MAX;
	Xsp[2][3] = 0;
	Xsp[3][0] = 0;
	Xsp[3][1] = 0;
	Xsp[3][2] = 0;
	Xsp[3][3] = 1;
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

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	GzMatrix Xpi, Xiw;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				Xpi[i][j] = 1;
			}
			else {
				Xpi[i][j] = 0;
			}
		}
	}
	float d = tan(m_camera.FOV / 2);
	Xpi[2][2] = d;
	Xpi[3][2] = d;

	GzCoord cameraXAxis, cameraYAxis, cameraZAxis;

	// Compute camera Z
	GzCoord cl;
	float modCL;
	cl[X] = m_camera.lookat[X] - m_camera.position[X];
	cl[Y] = m_camera.lookat[Y] - m_camera.position[Y];
	cl[Z] = m_camera.lookat[Z] - m_camera.position[Z];

	modCL = sqrt(cl[X] * cl[X] + cl[Y] * cl[Y] + cl[Z] * cl[Z]);

	cameraZAxis[X] = cl[X] / modCL;
	cameraZAxis[Y] = cl[Y] / modCL;
	cameraZAxis[Z] = cl[Z] / modCL;

	// Compute camera Y
	GzCoord up;
	float dotProductUpZ, modUp;
	dotProductUpZ = (m_camera.worldup[X] * cameraZAxis[X] +
		m_camera.worldup[Y] * cameraZAxis[Y] + m_camera.worldup[Z] * cameraZAxis[Z]);

	up[X] = m_camera.worldup[X] - (dotProductUpZ * cameraZAxis[X]);
	up[Y] = m_camera.worldup[Y] - (dotProductUpZ * cameraZAxis[Y]);
	up[Z] = m_camera.worldup[Z] - (dotProductUpZ * cameraZAxis[Z]);

	modUp = sqrt(up[X] * up[X] + up[Y] * up[Y] + up[Z] * up[Z]);

	cameraYAxis[X] = up[X] / modUp;
	cameraYAxis[Y] = up[Y] / modUp;
	cameraYAxis[Z] = up[Z] / modUp;

	// Compute camera X
	cameraXAxis[X] = (cameraYAxis[Y] * cameraZAxis[Z]) - (cameraYAxis[Z] * cameraZAxis[Y]);
	cameraXAxis[Y] = (cameraYAxis[Z] * cameraZAxis[X]) - (cameraYAxis[X] * cameraZAxis[Z]);
	cameraXAxis[Z] = (cameraYAxis[X] * cameraZAxis[Y]) - (cameraYAxis[Y] * cameraZAxis[X]);

	float dotProductXC, dotProductYC, dotProductZC;
	dotProductXC = cameraXAxis[X] * m_camera.position[X] +
		cameraXAxis[Y] * m_camera.position[Y] + cameraXAxis[Z] * m_camera.position[Z];
	dotProductYC = cameraYAxis[X] * m_camera.position[X] +
		cameraYAxis[Y] * m_camera.position[Y] + cameraYAxis[Z] * m_camera.position[Z];
	dotProductZC = cameraZAxis[X] * m_camera.position[X] +
		cameraZAxis[Y] * m_camera.position[Y] + cameraZAxis[Z] * m_camera.position[Z];

	Xiw[0][0] = cameraXAxis[X];
	Xiw[0][1] = cameraXAxis[Y];
	Xiw[0][2] = cameraXAxis[Z];
	Xiw[0][3] = -(dotProductXC);
	Xiw[1][0] = cameraYAxis[X];
	Xiw[1][1] = cameraYAxis[Y];
	Xiw[1][2] = cameraYAxis[Z];
	Xiw[1][3] = -(dotProductYC);
	Xiw[2][0] = cameraZAxis[X];
	Xiw[2][1] = cameraZAxis[Y];
	Xiw[2][2] = cameraZAxis[Z];
	Xiw[2][3] = -(dotProductZC);
	Xiw[3][0] = 0;
	Xiw[3][1] = 0;
	Xiw[3][2] = 0;
	Xiw[3][3] = 1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			m_camera.Xpi[i][j] = Xpi[i][j];
			m_camera.Xiw[i][j] = Xiw[i][j];

		}
	}

	GzPushMatrix(Xsp);
	GzPushMatrix(Xpi);
	GzPushMatrix(Xiw);

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/
	m_camera.position[0] = camera.position[0];
	m_camera.position[1] = camera.position[1];
	m_camera.position[2] = camera.position[2];
	m_camera.lookat[0] = camera.lookat[0];
	m_camera.lookat[1] = camera.lookat[1];
	m_camera.lookat[2] = camera.lookat[2];
	m_camera.worldup[0] = camera.worldup[0];
	m_camera.worldup[1] = camera.worldup[1];
	m_camera.worldup[2] = camera.worldup[2];
	m_camera.FOV = camera.FOV * (PI / 180);
	return GZ_SUCCESS;	
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	GzMatrix identityMat, result;

	if (matlevel > MATLEVELS) {
		return GZ_FAILURE;
	}
	if (matlevel == 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					identityMat[i][j] = 1;
				}
				else {
					identityMat[i][j] = 0;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i][j] = 0;
				for (int k = 0; k < 4; k++) {
					result[i][j] += identityMat[i][k] * matrix[k][j];
				}
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i][j] = 0;
				for (int k = 0; k < 4; k++) {
					result[i][j] += (Ximage[matlevel - 1])[i][k] * matrix[k][j];
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			(Ximage[matlevel])[i][j] = result[i][j];
		}
	}
	matlevel++;
	
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	if (matlevel < 0) {
		return GZ_FAILURE;
	}
	matlevel--;
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

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	
	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_POSITION) {
			float *position = (*(GzCoord *)valueList[0]);
			float v1X = *(position);
			float v1Y = *(position + 1);
			float v1Z = *(position + 2);

			float v2X = *(position + 3);
			float v2Y = *(position + 4);
			float v2Z = *(position + 5);

			float v3X = *(position + 6);
			float v3Y = *(position + 7);
			float v3Z = *(position + 8);

			float coordV1[4], coordV2[4], coordV3[4];

			coordV1[0] = v1X;
			coordV1[1] = v1Y;
			coordV1[2] = v1Z;
			coordV1[3] = 1;

			coordV2[0] = v2X;
			coordV2[1] = v2Y;
			coordV2[2] = v2Z;
			coordV2[3] = 1;

			coordV3[0] = v3X;
			coordV3[1] = v3Y;
			coordV3[2] = v3Z;
			coordV3[3] = 1;

			float resultX[4][1], resultY[4][1], resultZ[4][1];

			for (int i = 0; i < 4; i++) {
				resultX[i][0] = 0;
				for (int j = 0; j < 4; j++) {
					resultX[i][0] = resultX[i][0] + Ximage[matlevel - 1][i][j] * coordV1[j];
				}
			}

			for (int i = 0; i < 4; i++) {
				resultY[i][0] = 0;
				for (int j = 0; j < 4; j++) {
					resultY[i][0] = resultY[i][0] + Ximage[matlevel - 1][i][j] * coordV2[j];
				}
			}

			for (int i = 0; i < 4; i++) {
				resultZ[i][0] = 0;
				for (int j = 0; j < 4; j++) {
					resultZ[i][0] = resultZ[i][0] + Ximage[matlevel - 1][i][j] * coordV3[j];
				}
			}

			GzCoord v1, v2, v3;
			v1[X] = resultX[0][0] / resultX[3][0];
			v1[Y] = resultX[1][0] / resultX[3][0];
			v1[Z] = resultX[2][0] / resultX[3][0];

			v2[X] = resultY[0][0] / resultY[3][0];
			v2[Y] = resultY[1][0] / resultY[3][0];
			v2[Z] = resultY[2][0] / resultY[3][0];

			v3[X] = resultZ[0][0] / resultZ[3][0];
			v3[Y] = resultZ[1][0] / resultZ[3][0];
			v3[Z] = resultZ[2][0] / resultZ[3][0];
				   				    
			if ((v1[Z] < 0) || (v2[Z] < 0) || (v3[Z] < 0)) {
				return GZ_FAILURE;
			}


			float vertices[9] = { v1[X], v1[Y], v1[Z], v2[X], v2[Y], v2[Z], v3[X], v3[Y], v3[Z] };
			renderTriangle(vertices);
		}
	}
	
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


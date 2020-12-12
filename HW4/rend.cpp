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
	mat[1][1] = cos(degree * (PI / 180));
	mat[1][2] = -sin(degree * (PI / 180));
	mat[2][1] = sin(degree * (PI / 180));
	mat[2][2] = cos(degree * (PI / 180));

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

	mat[0][0] = cos(degree * (PI / 180));
	mat[0][2] = sin(degree * (PI / 180));
	mat[1][1] = 1;
	mat[2][0] = -sin(degree * (PI / 180));
	mat[2][2] = cos(degree * (PI / 180));
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

	mat[0][0] = cos(degree * (PI / 180));
	mat[0][1] = -sin(degree * (PI / 180));
	mat[1][0] = sin(degree * (PI / 180));
	mat[1][1] = cos(degree * (PI / 180));
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
	Xsp[1][1] = -(yRes / 2);
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

	interp_mode = GZ_RGB_COLOR;
	numlights = 0;
	GzColor ka = DEFAULT_AMBIENT;
	GzColor ks = DEFAULT_SPECULAR;
	GzColor kd = DEFAULT_DIFFUSE;

	Ka[RED] = ka[RED];
	Ka[GREEN] = ka[GREEN];
	Ka[BLUE] = ka[BLUE];

	Kd[RED] = kd[RED];
	Kd[GREEN] = kd[GREEN];
	Kd[BLUE] = kd[BLUE];

	Ks[RED] = ks[RED];
	Ks[GREEN] = ks[GREEN];
	Ks[BLUE] = ks[BLUE];

	spec = DEFAULT_SPEC;
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
	if (matlevel == 0) {
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

	if (matlevel == 1 || matlevel == 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Xnorm[matlevel][i][j] = identityMat[i][j];
			}
		}
	}
	else {
		GzMatrix unitaryResult, unitaryXNorm;
		unitaryResult[0][0] = matrix[0][0];
		unitaryResult[0][1] = matrix[0][1];
		unitaryResult[0][2] = matrix[0][2];
		unitaryResult[0][3] = 0;
		unitaryResult[1][0] = matrix[1][0];
		unitaryResult[1][1] = matrix[1][1];
		unitaryResult[1][2] = matrix[1][2];
		unitaryResult[1][3] = 0;
		unitaryResult[2][0] = matrix[2][0];
		unitaryResult[2][1] = matrix[2][1];
		unitaryResult[2][2] = matrix[2][2];
		unitaryResult[2][3] = 0;
		unitaryResult[3][0] = matrix[3][0];
		unitaryResult[3][1] = matrix[3][1];
		unitaryResult[3][2] = matrix[3][2];
		unitaryResult[3][3] = 1;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				unitaryXNorm[i][j] = 0;
				for (int k = 0; k < 4; k++) {
					unitaryXNorm[i][j] += Xnorm[matlevel - 1][i][k] * unitaryResult[k][j];
				}
			}
		}

		float modUnitaryXNorm = 1 / sqrt(unitaryXNorm[0][0] * unitaryXNorm[0][0] +
			unitaryXNorm[0][1] * unitaryXNorm[0][1] +
			unitaryXNorm[0][2] * unitaryXNorm[0][2]);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				unitaryXNorm[i][j] *= modUnitaryXNorm;
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Xnorm[matlevel][i][j] = unitaryXNorm[i][j];
			}
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

int GzRender::GzGet(int i, int j, GzIntensity* r, GzIntensity* g, GzIntensity* b, GzIntensity* a, GzDepth* z)
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

int GzRender::GzPutAttribute(int numAttributes, GzToken* nameList, GzPointer* valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/
	for (int i = 0; i < numAttributes; i++) {
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor* color = (GzColor*)valueList[i];
			flatcolor[RED] = (*color)[RED];
			flatcolor[GREEN] = (*color)[GREEN];
			flatcolor[BLUE] = (*color)[BLUE];
		}
		else if (nameList[i] == GZ_AMBIENT_COEFFICIENT) {
			GzColor* ka = (GzColor*)valueList[i];
			Ka[RED] = (*ka)[RED];
			Ka[GREEN] = (*ka)[GREEN];
			Ka[BLUE] = (*ka)[BLUE];
		}
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor* kd = (GzColor*)valueList[i];
			Kd[RED] = (*kd)[RED];
			Kd[GREEN] = (*kd)[GREEN];
			Kd[BLUE] = (*kd)[BLUE];
		}
		else if (nameList[i] == GZ_SPECULAR_COEFFICIENT) {
			GzColor* ks = (GzColor*)valueList[i];
			Ks[RED] = (*ks)[RED];
			Ks[GREEN] = (*ks)[GREEN];
			Ks[BLUE] = (*ks)[BLUE];
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight* al = (GzLight*)valueList[i];
			ambientlight.color[RED] = al->color[RED];
			ambientlight.color[GREEN] = al->color[GREEN];
			ambientlight.color[BLUE] = al->color[BLUE];
		}
		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT) {
			if (numlights > MAX_LIGHTS) {
				return GZ_FAILURE;
			}
			GzLight* dl = (GzLight*)valueList[i];
			lights[numlights].color[RED] = dl->color[RED];
			lights[numlights].color[GREEN] = dl->color[GREEN];
			lights[numlights].color[BLUE] = dl->color[BLUE];
			lights[numlights].direction[RED] = dl->direction[RED];
			lights[numlights].direction[GREEN] = dl->direction[GREEN];
			lights[numlights].direction[BLUE] = dl->direction[BLUE];
			numlights++;
		}
		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT) {
			float* s = (float*)valueList[i];
			spec = *s;
		}
		else if (nameList[i] == GZ_INTERPOLATE) {
			int* mode = (int*)valueList[i];
			interp_mode = *mode;
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken* nameList, GzPointer* valueList)
{
	GzCoord* positionList, *normalList;

	for (int i = 0; i < numParts; i++) {
		if (nameList[i] == GZ_NULL_TOKEN) {
			return GZ_FAILURE;
		}
		if (nameList[i] == GZ_POSITION) {
			positionList = static_cast<GzCoord*>(valueList[i]);
		}
		else if (nameList[i] == GZ_NORMALS) {
			normalList = static_cast<GzCoord*>(valueList[i]);
		}
	}

	// Get vertices
	float coordV1[4], coordV2[4], coordV3[4];

	coordV1[0] = positionList[0][0];
	coordV1[1] = positionList[0][1];
	coordV1[2] = positionList[0][2];
	coordV1[3] = 1;

	coordV2[0] = positionList[1][0];
	coordV2[1] = positionList[1][1];
	coordV2[2] = positionList[1][2];
	coordV2[3] = 1;

	coordV3[0] = positionList[2][0];
	coordV3[1] = positionList[2][1];
	coordV3[2] = positionList[2][2];
	coordV3[3] = 1;


	// Calculate the face normal
	GzCoord faceNormal = { 0, 0, 0 };
	faceNormal[X] = ((coordV2[Y] - coordV1[Y]) * (coordV3[Z] - coordV1[Z])) - ((coordV3[Y] - coordV1[Y]) * (coordV2[Z] - coordV1[Z]));
	faceNormal[Y] = ((coordV2[Z] - coordV1[Z]) * (coordV3[X] - coordV1[X])) - ((coordV3[Z] - coordV1[Z]) * (coordV2[X] - coordV1[X]));
	faceNormal[Z] = ((coordV2[X] - coordV1[X]) * (coordV3[Y] - coordV1[Y])) - ((coordV3[X] - coordV1[X]) * (coordV2[Y] - coordV1[Y]));

	// Transform the face normal using Xnorm
	GzCoord transformedFaceNormal;
	transformedFaceNormal[X] = faceNormal[X] * Xnorm[matlevel - 1][0][0] +
		faceNormal[Y] * Xnorm[matlevel - 1][0][1] +
		faceNormal[Z] * Xnorm[matlevel - 1][0][2];

	transformedFaceNormal[Y] = faceNormal[X] * Xnorm[matlevel - 1][1][0] +
		faceNormal[Y] * Xnorm[matlevel - 1][1][1] +
		faceNormal[Z] * Xnorm[matlevel - 1][1][2];

	transformedFaceNormal[Z] = faceNormal[X] * Xnorm[matlevel - 1][2][0] +
		faceNormal[Y] * Xnorm[matlevel - 1][2][1] +
		faceNormal[Z] * Xnorm[matlevel - 1][2][2];

	// Normalize the face normal
	float modTransformedFaceNormal = sqrt((transformedFaceNormal[0] * transformedFaceNormal[0]) + (transformedFaceNormal[1] * transformedFaceNormal[1]) + (transformedFaceNormal[2] * transformedFaceNormal[2]));
	faceNormal[0] = transformedFaceNormal[0] / modTransformedFaceNormal;
	faceNormal[1] = transformedFaceNormal[1] / modTransformedFaceNormal;
	faceNormal[2] = transformedFaceNormal[2] / modTransformedFaceNormal;

	// Transform the vertices using Ximage
	float transformedV1[4][1], transformedV2[4][1], transformedV3[4][1];
	for (int i = 0; i < 4; i++) {
		transformedV1[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedV1[i][0] = transformedV1[i][0] + ((Ximage[matlevel - 1][i][j]) * coordV1[j]);
		}
	}

	for (int i = 0; i < 4; i++) {
		transformedV2[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedV2[i][0] = transformedV2[i][0] + ((Ximage[matlevel - 1][i][j]) * coordV2[j]);
		}
	}

	for (int i = 0; i < 4; i++) {
		transformedV3[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedV3[i][0] = transformedV3[i][0] + ((Ximage[matlevel - 1][i][j]) * coordV3[j]);
		}
	}

	// Calcuate vertices
	GzCoord V1, V2, V3;
	V1[X] = transformedV1[0][0] / transformedV1[3][0];
	V1[Y] = transformedV2[0][0] / transformedV2[3][0];
	V1[Z] = transformedV3[0][0] / transformedV3[3][0];

	V2[X] = transformedV1[1][0] / transformedV1[3][0];
	V2[Y] = transformedV2[1][0] / transformedV2[3][0];
	V2[Z] = transformedV3[1][0] / transformedV3[3][0];

	V3[X] = transformedV1[2][0] / transformedV1[3][0];
	V3[Y] = transformedV2[2][0] / transformedV2[3][0];
	V3[Z] = transformedV3[2][0] / transformedV3[3][0];

	// Get normals
	float coordN1[4], coordN2[4], coordN3[4];

	coordN1[0] = normalList[0][0];
	coordN1[1] = normalList[0][1];
	coordN1[2] = normalList[0][2];
	coordN1[3] = 1;

	coordN2[0] = normalList[1][0];
	coordN2[1] = normalList[1][1];
	coordN2[2] = normalList[1][2];
	coordN2[3] = 1;

	coordN3[0] = normalList[2][0];
	coordN3[1] = normalList[2][1];
	coordN3[2] = normalList[2][2];
	coordN3[3] = 1;

	// Transform normals using Xnorm
	float transformedN1[4][1], transformedN2[4][1], transformedN3[4][1];
	for (int i = 0; i < 4; i++) {
		transformedN1[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedN1[i][0] = transformedN1[i][0] + ((Xnorm[matlevel - 1][i][j]) * coordN1[j]);
		}
	}
	for (int i = 0; i < 4; i++) {
		transformedN2[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedN2[i][0] = transformedN2[i][0] + ((Xnorm[matlevel - 1][i][j]) * coordN2[j]);
		}
	}
	for (int i = 0; i < 4; i++) {
		transformedN3[i][0] = 0;
		for (int j = 0; j < 4; j++) {
			transformedN3[i][0] = transformedN3[i][0] + ((Xnorm[matlevel - 1][i][j]) * coordN3[j]);
		}
	}

	// Calculate normals
	GzCoord N1, N2, N3;
	N1[X] = transformedN1[0][0];
	N1[Y] = transformedN1[0][1];
	N1[Z] = transformedN1[0][2];

	N2[X] = transformedN2[0][0];
	N2[Y] = transformedN2[0][1];
	N2[Z] = transformedN2[0][2];

	N3[X] = transformedN3[0][0];
	N3[Y] = transformedN3[0][1];
	N3[Z] = transformedN3[0][2];

	// Normalize normals
	float modN1 = sqrt((N1[X] * N1[X]) + (N1[Y] * N1[Y]) + (N1[Z] * N1[Z]));
	float modN2 = sqrt((N2[X] * N2[X]) + (N2[Y] * N2[Y]) + (N2[Z] * N2[Z]));
	float modN3 = sqrt((N3[X] * N3[X]) + (N3[Y] * N3[Y]) + (N3[Z] * N3[Z]));

	N1[X] = N1[X] / modN1;
	N1[Y] = N1[Y] / modN1;
	N1[Z] = N1[Z] / modN1;

	N2[X] = N2[X] / modN2;
	N2[Y] = N2[Y] / modN2;
	N2[Z] = N2[Z] / modN2;

	N3[X] = N3[X] / modN3;
	N3[Y] = N3[Y] / modN3;
	N3[Z] = N3[Z] / modN3;

	GzColor colorV1 = { 0, 0, 0 };
	GzColor colorV2 = { 0, 0, 0 };
	GzColor colorV3 = { 0, 0, 0 };

	// Get color values for each normal given each vertex
	getColorValue(N1, colorV1);
	getColorValue(N2, colorV2);
	getColorValue(N3, colorV3);

	// Clip triangles
	if ((V1[X] < 0 || V1[X] > (xres)) && (V1[Y] < 0 || V1[Y] > (xres)) && (V1[Z] < 0 || V1[Z] > (xres))) {
		return GZ_FAILURE;

	}

	if ((V2[X] < 0 || V2[X] > (yres)) && (V2[Y] < 0 || V2[Y] > (yres)) && (V2[Z] < 0 || V2[Z] > (yres))) {
		return GZ_FAILURE;

	}

	if (V3[X] < 0 && V3[Y] < 0 && V3[Z] < 0) {
		return GZ_FAILURE;

	}

	float yMin, yMax;

	// Sort on V2
	sortOnV2(V1, V2, V3, colorV1, colorV2, colorV3, N1, N2, N3);

	// Minimum and maximum V2 for bounding box
	yMin = V2[0];
	yMax = V2[2];
	float temp;

	if ((V2[1] == V2[0]) || (V1[1] < V1[0]))
	{
		// Swap x and y for V1, V2 and V3
		temp = V1[0];
		V1[0] = V1[1];
		V1[1] = temp;

		temp = V2[0];
		V2[0] = V2[1];
		V2[1] = temp;

		temp = V3[0];
		V3[0] = V3[1];
		V3[1] = temp;

		// Swap color for V1 and V2
		temp = colorV1[RED];
		colorV1[RED] = colorV2[RED];
		colorV2[RED] = temp;

		temp = colorV1[GREEN];
		colorV1[GREEN] = colorV2[GREEN];
		colorV2[GREEN] = temp;

		temp = colorV1[BLUE];
		colorV1[BLUE] = colorV2[BLUE];
		colorV2[BLUE] = temp;

		// Swap N1 and N2
		temp = N1[X];
		N1[X] = N2[X];
		N2[X] = temp;

		temp = N1[Y];
		N1[Y] = N2[Y];
		N2[Y] = temp;

		temp = N1[Z];
		N1[Z] = N2[Z];
		N2[Z] = temp;
	}

	else if ((V2[1] == V2[2]) || (V1[1] > V1[2])) {
		// Swap y and z for V1, V2 and V3
		temp = V1[1];
		V1[1] = V1[2];
		V1[2] = temp;

		temp = V2[1];
		V2[1] = V2[2];
		V2[2] = temp;

		temp = V3[1];
		V3[1] = V3[2];
		V3[2] = temp;

		// Swap color V2 and V3
		temp = colorV2[RED];
		colorV2[RED] = colorV3[RED];
		colorV3[RED] = temp;

		temp = colorV2[GREEN];
		colorV2[GREEN] = colorV3[GREEN];
		colorV3[GREEN] = temp;

		temp = colorV2[BLUE];
		colorV2[BLUE] = colorV3[BLUE];
		colorV3[BLUE] = temp;

		// Swap N2 and N3
		temp = N2[X];
		N2[X] = N3[X];
		N3[X] = temp;

		temp = N2[Y];
		N2[Y] = N3[Y];
		N3[Y] = temp;

		temp = N2[Z];
		N2[Z] = N3[Z];
		N3[Z] = temp;

	}

	float coefficientA = (V2[2] - V2[0]);
	float coefficientB = -(V1[2] - V1[0]);
	float coefficientC = (-coefficientB * V2[0]) - (coefficientA * V1[0]);

	float vert = (-coefficientB * V2[1] - coefficientC) / coefficientA;
	if (vert < V1[1]) {
		temp = V2[2];
		V2[2] = V2[1];
		V2[1] = temp;

		temp = V1[2];
		V1[2] = V1[1];
		V1[1] = temp;

		temp = V3[2];
		V3[2] = V3[1];
		V3[1] = temp;

		temp = colorV2[RED];
		colorV2[RED] = colorV3[RED];
		colorV3[RED] = temp;

		temp = colorV2[GREEN];
		colorV2[GREEN] = colorV3[GREEN];
		colorV3[GREEN] = temp;

		temp = colorV2[BLUE];
		colorV2[BLUE] = colorV3[BLUE];
		colorV3[BLUE] = temp;


		temp = N2[X];
		N2[X] = N3[X];
		N3[X] = temp;

		temp = N2[Y];
		N2[Y] = N3[Y];
		N3[Y] = temp;

		temp = N2[Z];
		N2[Z] = N3[Z];
		N3[Z] = temp;
	}


	// Find minimum  and maximum V1 for bounding box
	float xMin, xMax;

	if (V1[0] < V1[1] && V1[0] < V1[2]) {
		xMin = V1[0];

	}
	else {
		if (V1[1] < V1[2] && V1[1] < V1[0]) {
			xMin = V1[1];
		}
		else {
			xMin = V1[2];
		}
	}


	if (V1[0] > V1[1] && V1[0] > V1[2]) {
		xMax = V1[0];

	}
	else {
		if (V1[1] > V1[2] && V1[1] > V1[0]) {
			xMax = V1[1];
		}
		else {
			xMax = V1[2];
		}
	}

	// Clamp xMin, xMax, yMin, yMax between 0 and 255
	if (xMin < 0) {
		xMin = 0;

	}
	if (xMin > 255) {
		xMin = 255;

	}
	if (xMax < 0) {
		xMax = 0;

	}
	if (xMax > 255) {
		xMax = 255;

	}
	if (yMin < 0) {
		yMin = 0;

	}
	if (yMin > 255) {
		yMin = 255;

	}
	if (yMax < 0) {
		yMax = 0;

	}
	if (yMax > 255) {
		yMax = 255;

	}

	float coefficientA1 = (V2[1] - V2[0]);
	float coefficientA2 = (V2[2] - V2[1]);
	float coefficientA3 = (V2[0] - V2[2]);

	float coefficientB1 = -(V1[1] - V1[0]);
	float coefficientB2 = -(V1[2] - V1[1]);
	float coefficientB3 = -(V1[0] - V1[2]);

	float coefficientC1 = ((-(coefficientB1)*V2[1]) - (coefficientA1 * V1[1]));
	float coefficientC2 = ((-(coefficientB2)*V2[2]) - (coefficientA2 * V1[2]));
	float coefficientC3 = ((-(coefficientB3)*V2[0]) - (coefficientA3 * V1[0]));
	float d1, d2, d3;

	float A, B, C, D;
	float x11, y11, z11, x22, y22, z22;
	x11 = (V1[1] - V1[0]);
	y11 = (V2[1] - V2[0]);
	z11 = (V3[1] - V3[0]);
	x22 = (V1[2] - V1[0]);
	y22 = (V2[2] - V2[0]);
	z22 = (V3[2] - V3[0]);
	A = ((y11 * z22) - (y22 * z11));
	B = ((x22 * z11) - (x11 * z22));
	C = ((x11 * y22) - (x22 * y11));
	D = ((V2[0] * (B)) + (V1[0] * (A)) + (V3[0] * (C))) * (-1);

	GzColor gouraudA, gouraudB, gouraudC, gouraudD;

	gouraudA[0] = ((y11 * (colorV3[0] - colorV1[0])) - (y22 * (colorV2[0] - colorV1[0])));
	gouraudB[0] = ((x22 * (colorV2[0] - colorV1[0])) - (x11 * (colorV3[0] - colorV1[0])));
	gouraudC[0] = ((x11 * y22) - (x22 * y11));
	gouraudD[0] = ((V2[0] * gouraudB[0]) + (V1[0] * gouraudA[0]) + (colorV1[0] * gouraudC[0])) * (-1);

	gouraudA[1] = ((y11 * (colorV3[1] - colorV1[1])) - (y22 * (colorV2[1] - colorV1[1])));
	gouraudB[1] = ((x22 * (colorV2[1] - colorV1[1])) - (x11 * (colorV3[1] - colorV1[1])));
	gouraudC[1] = ((x11 * y22) - (x22 * y11));
	gouraudD[1] = ((V2[0] * gouraudB[1]) + (V1[0] * gouraudA[1]) + (colorV1[1] * gouraudC[1])) * (-1);

	gouraudA[2] = ((y11 * (colorV3[2] - colorV1[2])) - (y22 * (colorV2[2] - colorV1[2])));
	gouraudB[2] = ((x22 * (colorV2[2] - colorV1[2])) - (x11 * (colorV3[2] - colorV1[2])));
	gouraudC[2] = ((x11 * y22) - (x22 * y11));
	gouraudD[2] = ((V2[0] * gouraudB[2]) + (V1[0] * gouraudA[2]) + (colorV1[2] * gouraudC[2])) * (-1);

	int zInterpolate = 0;
	GzColor gouraudZInterpolate;
	for (int i = floor(xMin); i < ceil(xMax); i++) {
		for (int j = floor(yMin); j < ceil(yMax); j++) {
			d1 = (coefficientA1 * i) + (coefficientB1 * j) + coefficientC1;
			d2 = (coefficientA2 * i) + (coefficientB2 * j) + coefficientC2;
			d3 = (coefficientA3 * i) + (coefficientB3 * j) + coefficientC3;
			zInterpolate = -(A * i + B * j + D) / C;

			if (!(d1 < 0 || d2 < 0 || d3 < 0)) {
				// Validate for Z Buffer
				GzDepth zValue;
				int index = ARRAY(i, j);
				zValue = (pixelbuffer + index)->z;
				if (zInterpolate < zValue || zValue == 0)
				{
					if (interp_mode == GZ_COLOR) {
						gouraudShader(i, j,
							gouraudA, gouraudB, gouraudC, gouraudD, gouraudZInterpolate);
					}
					else if (interp_mode == GZ_NORMALS) {
						phongShader(i, j, V1, V2, V3, N1, N2, N3,
							x11, y11, z11, x22, y22, z22,
							colorV1, colorV2, colorV3);
					}
					else if (interp_mode == GZ_FLAT) {
						flatShader(faceNormal);
					}

					// Clamp flat color between 0 and 1
					if (flatcolor[0] > 1) {
						flatcolor[0] = 1;
					}

					if (flatcolor[0] < 0) {
						flatcolor[0] = 0;
					}

					if (flatcolor[1] > 1) {
						flatcolor[1] = 1;
					}

					if (flatcolor[1] < 0) {
						flatcolor[1] = 0;
					}

					if (flatcolor[2] > 1) {
						flatcolor[2] = 1;
					}

					if (flatcolor[2] < 0) {
						flatcolor[2] = 0;
					}

					GzPut(i, j, ctoi(flatcolor[RED]), ctoi(flatcolor[GREEN]), ctoi(flatcolor[BLUE]), 1, zInterpolate);
				}
			}
		}
	}
	return GZ_SUCCESS;
}

void GzRender::flatShader(GzCoord faceNormal) {
	GzColor colorFlat = { 0, 0, 0 };

	getColorValue(faceNormal, colorFlat);
	flatcolor[0] = colorFlat[0];
	flatcolor[1] = colorFlat[1];
	flatcolor[2] = colorFlat[2];
}

void GzRender::gouraudShader(int i, int j,
	GzColor gouraudA, GzColor gouraudB, GzColor gouraudC, GzColor gouraudD, GzColor gouraudZInterpolate) {
	gouraudZInterpolate[0] = -(gouraudA[0] * i + gouraudB[0] * j + gouraudD[0]) / gouraudC[0];
	gouraudZInterpolate[1] = -(gouraudA[1] * i + gouraudB[1] * j + gouraudD[1]) / gouraudC[1];
	gouraudZInterpolate[2] = -(gouraudA[2] * i + gouraudB[2] * j + gouraudD[2]) / gouraudC[2];

	flatcolor[0] = gouraudZInterpolate[0];
	flatcolor[1] = gouraudZInterpolate[1];
	flatcolor[2] = gouraudZInterpolate[2];
}

void GzRender::phongShader(int i, int j, GzCoord V1, GzCoord V2, GzCoord V3, GzCoord N1, GzCoord N2, GzCoord N3,
	float x11, float y11, float z11, float x22, float y22, float z22,
	GzColor colorV1, GzColor colorV2, GzColor colorV3) {
	GzColor phongA, phongB, phongC, phongD;

	phongA[0] = ((y11 * (N3[0] - N1[0])) - (y22 * (N2[0] - N1[0])));
	phongB[0] = ((x22 * (N2[0] - N1[0])) - (x11 * (N3[0] - N1[0])));
	phongC[0] = ((x11 * y22) - (x22 * y11));
	phongD[0] = ((V2[0] * phongB[0]) + (V1[0] * phongA[0]) + (N1[0] * phongC[0])) * (-1);

	phongA[1] = ((y11 * (N3[1] - N1[1])) - (y22 * (N2[1] - N1[1])));
	phongB[1] = ((x22 * (N2[1] - N1[1])) - (x11 * (N3[1] - N1[1])));;
	phongC[1] = ((x11 * y22) - (x22 * y11));
	phongD[1] = ((V2[0] * phongB[1]) + (V1[0] * phongA[1]) + (N1[1] * phongC[1])) * (-1);

	phongA[2] = ((y11 * (N3[2] - N1[2])) - (y22 * (N2[2] - N1[2])));
	phongB[2] = ((x22 * (N2[2] - N1[2])) - (x11 * (N3[2] - N1[2])));;
	phongC[2] = ((x11 * y22) - (x22 * y11));
	phongD[2] = ((V2[0] * phongB[2]) + (V1[0] * phongA[2]) + (N1[2] * phongC[2])) * (-1);

	GzColor phongZinterpolate;

	phongZinterpolate[0] = -(phongA[0] * i + phongB[0] * j + phongD[0]) / phongC[0];
	phongZinterpolate[1] = -(phongA[1] * i + phongB[1] * j + phongD[1]) / phongC[1];
	phongZinterpolate[2] = -(phongA[2] * i + phongB[2] * j + phongD[2]) / phongC[2];

	float modPhongZinterpolate = sqrt((phongZinterpolate[0] * phongZinterpolate[0]) + (phongZinterpolate[1] * phongZinterpolate[1]) + (phongZinterpolate[2] * phongZinterpolate[2]));
	phongZinterpolate[0] = phongZinterpolate[0] / modPhongZinterpolate;
	phongZinterpolate[1] = phongZinterpolate[1] / modPhongZinterpolate;
	phongZinterpolate[2] = phongZinterpolate[2] / modPhongZinterpolate;

	GzColor colorNormal;
	getColorValue(phongZinterpolate, colorNormal);

	flatcolor[0] = colorNormal[0];
	flatcolor[1] = colorNormal[1];
	flatcolor[2] = colorNormal[2];
}

int GzRender::getColorValue(GzCoord normal, GzColor color) {
	GzCoord E, R;
	E[0] = 0;
	E[1] = 0;
	E[2] = -1;

	float dotProductNE, dotProductNL, dotProductRE;

	GzColor diffuseColorValue, specularColorValue;
	diffuseColorValue[RED] = 0;
	diffuseColorValue[GREEN] = 0;
	diffuseColorValue[BLUE] = 0;
	specularColorValue[RED] = 0;
	specularColorValue[GREEN] = 0;
	specularColorValue[BLUE] = 0;
	dotProductNE = ((normal[0] * E[0]) + (normal[1] * E[1]) + (normal[2] * E[2]));

	GzCoord newNormal = { 0, 0, 0 };

	for (int i = 0; i < (numlights); i++) {
		// Calculate dot product of N and L
		dotProductNL = ((normal[0] * (lights[i].direction[0])) + (normal[1] * (lights[i].direction[1])) + (normal[2] * (lights[i].direction[2])));

		if (dotProductNL > 0 && dotProductNE > 0) {
			// Calculate R
			R[0] = (2 * (dotProductNL)*normal[0]) - (lights[i].direction[0]);
			R[1] = (2 * (dotProductNL)*normal[1]) - (lights[i].direction[1]);
			R[2] = (2 * (dotProductNL)*normal[2]) - (lights[i].direction[2]);
		}
		else if (dotProductNL < 0 && dotProductNE < 0) {
			// Invert normals
			newNormal[0] = (-1 * normal[0]);
			newNormal[1] = (-1 * normal[1]);
			newNormal[2] = (-1 * normal[2]);

			// Calculate dot product of N and L again
			dotProductNL = ((newNormal[0] * (lights[i].direction[0])) + (newNormal[1] * (lights[i].direction[1])) + (newNormal[2] * (lights[i].direction[2])));

			// Calculate R
			R[0] = (2 * (dotProductNL)*newNormal[0]) - (lights[i].direction[0]);
			R[1] = (2 * (dotProductNL)*newNormal[1]) - (lights[i].direction[1]);
			R[2] = (2 * (dotProductNL)*newNormal[2]) - (lights[i].direction[2]);
		}
		else {
			continue;
		}

		// normalize R
		float modR = sqrt((R[0] * R[0]) + (R[1] * R[1]) + (R[2] * R[2]));
		R[0] = R[0] / modR;
		R[1] = R[1] / modR;
		R[2] = R[2] / modR;

		// Calculate dot product of R and E
		dotProductRE = ((R[0] * E[0]) + (R[1] * E[1]) + (R[2] * E[2]));

		// Clamp the dot product between 0 and 1
		if (dotProductRE < 0) {
			dotProductRE = 0;
		}
		else if (dotProductRE > 1) {
			dotProductRE = 1;
		}

		// Calculate diffuse color
		diffuseColorValue[RED] = diffuseColorValue[RED] + ((dotProductNL) * (lights[i].color[RED]));
		diffuseColorValue[GREEN] = diffuseColorValue[GREEN] + ((dotProductNL) * (lights[i].color[GREEN]));
		diffuseColorValue[BLUE] = diffuseColorValue[BLUE] + ((dotProductNL) * (lights[i].color[BLUE]));

		// Calculate specular color
		specularColorValue[RED] = specularColorValue[RED] + ((pow(dotProductRE, spec)) * (lights[i].color[RED]));
		specularColorValue[GREEN] = specularColorValue[GREEN] + ((pow(dotProductRE, spec)) * (lights[i].color[GREEN]));
		specularColorValue[BLUE] = specularColorValue[BLUE] + ((pow(dotProductRE, spec)) * (lights[i].color[BLUE]));

	}

	// Calculate Color = Specular + Diffuse + Ambient;
	color[RED] = (Ks[RED] * specularColorValue[RED]) + (Kd[RED] * diffuseColorValue[RED]) + (Ka[RED] * ambientlight.color[RED]);
	color[GREEN] = (Ks[GREEN] * specularColorValue[GREEN]) + (Kd[GREEN] * diffuseColorValue[GREEN]) + (Ka[GREEN] * ambientlight.color[GREEN]);
	color[BLUE] = (Ks[BLUE] * specularColorValue[BLUE]) + (Kd[BLUE] * diffuseColorValue[BLUE]) + (Ka[BLUE] * ambientlight.color[BLUE]);

	return GZ_SUCCESS;
}

void GzRender::sortOnV2(GzCoord V1, GzCoord V2, GzCoord V3, GzColor colorV1, GzColor colorV2, GzColor colorV3, GzCoord N1, GzCoord N2, GzCoord N3) {
	float tempVariable;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2 - i; j++) {
			if (V2[j] > V2[j + 1]) {
				// Swap X and Y for all vertices  
				tempVariable = V1[j];
				V1[j] = V1[j + 1];
				V1[j + 1] = tempVariable;

				tempVariable = V2[j];
				V2[j] = V2[j + 1];
				V2[j + 1] = tempVariable;

				tempVariable = V3[j];
				V3[j] = V3[j + 1];
				V3[j + 1] = tempVariable;

				if (j == 1) {
					// Swap colors for V2 and V3
					tempVariable = colorV2[RED];
					colorV2[RED] = colorV3[RED];
					colorV3[RED] = tempVariable;

					tempVariable = colorV2[GREEN];
					colorV2[GREEN] = colorV3[GREEN];
					colorV3[GREEN] = tempVariable;

					tempVariable = colorV2[BLUE];
					colorV2[BLUE] = colorV3[BLUE];
					colorV3[BLUE] = tempVariable;

					// Swap normals N2 and N3
					tempVariable = N2[X];
					N2[X] = N3[X];
					N3[X] = tempVariable;

					tempVariable = N2[Y];
					N2[Y] = N3[Y];
					N3[Y] = tempVariable;

					tempVariable = N2[Z];
					N2[Z] = N3[Z];
					N3[Z] = tempVariable;
				}
				if (j == 0) {
					// Swap colors for V1 and V2
					tempVariable = colorV1[RED];
					colorV1[RED] = colorV2[RED];
					colorV2[RED] = tempVariable;

					tempVariable = colorV1[GREEN];
					colorV1[GREEN] = colorV2[GREEN];
					colorV2[GREEN] = tempVariable;

					tempVariable = colorV1[BLUE];
					colorV1[BLUE] = colorV2[BLUE];
					colorV2[BLUE] = tempVariable;

					// Swap normals N1 and N2
					tempVariable = N1[X];
					N1[X] = N2[X];
					N2[X] = tempVariable;

					tempVariable = N1[Y];
					N1[Y] = N2[Y];
					N2[Y] = tempVariable;

					tempVariable = N1[Z];
					N1[Z] = N2[Z];
					N2[Z] = tempVariable;

				}
			}
		}
	}
}


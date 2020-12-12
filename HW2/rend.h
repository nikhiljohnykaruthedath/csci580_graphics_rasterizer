
#include	"gz.h"
#ifndef GZRENDER_
#define GZRENDER_


/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	10		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

/* Dummy definition : change it later */
#ifndef GzCamera
#define GzCamera	    	GzPointer
#endif

#ifndef GzLight
#define GzLight		GzPointer
#endif

#ifndef GzTexture
#define GzTexture	GzPointer
#endif


struct edgeDDA {
	GzCoord start, end, current;
	float slopeX, slopeZ;
};

class GzRender{			/* define a renderer */
  

public:
	unsigned short	xres;
	unsigned short	yres;
	GzPixel		*pixelbuffer;		/* frame buffer array */
	char* framebuffer;

	GzCamera		camera;
	short		matlevel;	/* top of stack - current xform */
	GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
	GzMatrix		Xnorm[MATLEVELS];	/* xform for norms (Xim) */
	GzMatrix		Xsp;		/* NDC to screen (pers-to-screen) */
	GzColor		flatcolor;    /* color state for flat shaded triangles */
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		spec;		/* specular power */
	GzTexture		tex_fun;  /* tex_fun(float u, float v, GzColor color) */
	GzCoord v1, v2, v3;
	struct edgeDDA eV1V2, eV2V3, eV1V3;

	// Constructors
	GzRender(int xRes, int yRes);
	~GzRender();

	// HW1: Display methods
	int GzDefault();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();

	// HW2: Render methods
	int GzPutAttribute(int numAttributes, GzToken *nameList, GzPointer *valueList);
	int GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList);
	void renderTriangle(float *position);
	void renderLeftTriangle();
	void renderRightTriangle();
	void sortVertices(float *positions);
	float determineLeftRightEdge(GzCoord v1, GzCoord v2, float y);
	void assignEdges();
	void updateTopEdges(float deltaY);
	void updateBottomEdges(float deltaY);
	float findSlope(char axis, GzCoord v1, GzCoord v2);
	inline float findDelta(float coord) { return ceil(coord) - coord; }

	// Extra methods: NOT part of API - just for general assistance */
	inline int ARRAY(int x, int y){return x+(y*xres);}	/* simplify fbuf indexing */
	inline short	ctoi(float color) {return(short)((int)(color * ((1 << 12) - 1)));}		/* convert float color to GzIntensity short */
};

#endif


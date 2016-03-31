

#ifndef VIEWERDRAW_INCLUDED
#define VIEWERDRAW_INCLUDED

//
// Functions for drawing (OpenGL) objects that
// can be seen in the viewer
//

#include <vector>
#include "tgeometry.h"

// forward declaration
class SceneViewer;
class Ruler;
class TAffine;

//=============================================================================
// ViewerDraw
//-----------------------------------------------------------------------------

namespace ViewerDraw
{

enum { // cf. drawCamera()
	CAMERA_REFERENCE = 0X1,
	CAMERA_3D = 0X2,
	SAFE_AREA = 0X4,
	SOLID_LINE = 0X8,
	SUBCAMERA = 0X10
};

TRectD getCameraRect();

void drawCameraMask(SceneViewer *viewer);
void drawGridAndGuides(
	SceneViewer *viewer,
	double viewerScale,
	Ruler *vRuler, Ruler *hRuler,
	bool gridEnabled);

void draw3DCamera(unsigned long flags, double zmin, double phi);
void drawCamera(unsigned long flags, double pixelSize);

void draw3DFrame(double zmin, double phi);
void drawDisk(int &tableDLId);
void drawFieldGuide();
void drawColorcard(UCHAR channel);

void drawSafeArea();

unsigned int createDiskDisplayList();

} // namespace

#endif

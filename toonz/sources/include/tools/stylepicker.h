

#ifndef STYLE_PICKER_H
#define STYLE_PICKER_H

//#include "timage.h"
#include "tcommon.h"
#include "tpalette.h"

class TStroke;

#undef DVAPI
#undef DVVAR
#ifdef TNZTOOLS_EXPORTS
#define DVAPI DV_EXPORT_API
#define DVVAR DV_EXPORT_VAR
#else
#define DVAPI DV_IMPORT_API
#define DVVAR DV_IMPORT_VAR
#endif

class DVAPI StylePicker
{
	TImageP m_image;
	TPaletteP m_palette;

public:
	StylePicker() {}

	// Use image's palette as palette
	StylePicker(const TImageP &image);

	// External palette (eg. if image is a raster)
	StylePicker(const TImageP &image, const TPaletteP &palette);

	// pickStyleId(point, radius)
	//
	// point is expressed in inches (possibly using DPI of image)
	// point == (0,0) indicates center of the image
	//
	// tzp images:
	//   return index of the pixel color identified by point
	//   (radius is ignored)
	//   if tone<maxtone/2, returns the ink or the paint
	//
	// pli images:
	//   return colorstyle index of the stroke closest to the point (but not
	//   beyond the radius) or of the fill area that includes the point
	//
	// fullcolor images:
	//   return colorstyle index of the palette provided in the constructor
	//   whose main color is close to the color of the pixel identified by the
	//   point (radius is ignored)
	//
	// Note: Returns -1 if nothing is found

	/*-- (In StylePickerTool) Can switch picker between Line and Area. mode: 0=Area 1=Line, 2=Line&Areas (default) --*/
	int pickStyleId(const TPointD &point, double radius2 = 1, int mode = 2) const;

	/*--- Pick up tone of Toonz raster level. ---*/
	int pickTone(const TPointD &pos);

	// For pli: as above, but returns the maincolor
	// For tzp and fullcolor: returns actual color of the pixel
	TPixel32 pickColor(const TPointD &point, double radius2 = 1) const;

	// Returns average color of the current openGL window
	TPixel32 pickColor(const TRectD &area) const;

	// Returns average color in the current stroke of the current openGL window
	TPixel32 pickColor(TStroke *stroke) const;

	// helper function. coordinate conversion:
	// p in the reference system described above
	// return value refers to the possible raster: (0,0)==leftbottom
	TPoint getRasterPoint(const TPointD &p) const;
};

#endif

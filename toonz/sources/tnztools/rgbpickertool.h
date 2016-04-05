#ifndef RGBPICKERTOOL_H
#define RGBPICKERTOOL_H

#include "tools/tool.h"
#include "tproperty.h"
#include "toonz/strokegenerator.h"
#include "tools/tooloptions.h"

#include <QCoreApplication>

class RGBPickerTool : public TTool
{
	Q_DECLARE_TR_FUNCTIONS(RGBPickerTool)

	bool m_firstTime;
	int m_currentStyleId;
	TPixel32 m_oldValue, m_currentValue;

	TRectD m_selectingRect;
	TRectD m_drawingRect;
	TPropertyGroup m_prop;
	TEnumProperty m_pickType;

	TBoolProperty m_passivePick;
	vector<RGBPickerToolOptionsBox *> m_toolOptionsBox;

	//Added to draw polyline lasso
	StrokeGenerator m_drawingTrack;
	StrokeGenerator m_workingTrack;
	TPointD m_firstDrawingPos, m_firstWorkingPos;
	TPointD m_mousePosition;
	double m_thick;
	TStroke *m_stroke;
	TStroke *m_firstStroke;
	vector<TPointD> m_drawingPolyline;
	vector<TPointD> m_workingPolyline;
	bool m_makePick;

public:
	RGBPickerTool();

	/*-- ToolOptionBox上にPassiveに拾った色を表示するため --*/
	void setToolOptionsBox(RGBPickerToolOptionsBox *toolOptionsBox);

	ToolType getToolType() const { return TTool::LevelReadTool; }

	void updateTranslation();

	// Used to notify and set the currentColor outside the draw() methods:
	// using special style there was a conflict between the draw() methods of the tool
	// and the genaration of the icon inside the style editor (makeIcon()) which use
	// another glContext

	void onImageChanged();

	void draw();

	void leftButtonDown(const TPointD &pos, const TMouseEvent &e);

	void leftButtonDrag(const TPointD &pos, const TMouseEvent &e);

	void leftButtonUp(const TPointD &pos, const TMouseEvent &);

	void leftButtonDoubleClick(const TPointD &pos, const TMouseEvent &e);

	void mouseMove(const TPointD &pos, const TMouseEvent &e);

	void pick(TPoint pos);

	void pickRect();

	void pickStroke();

	bool onPropertyChanged(string propertyName);

	void onActivate();

	TPropertyGroup *getProperties(int targetType);

	int getCursorId() const;

	void doPolylinePick();

	//!Add \b pos to \b m_track and draw first part of the lasso. Initializes \b m_firstPos
	void startFreehand(const TPointD &drawingPos, const TPointD &workingPos);

	//!Add \b pos to \b m_track and draw another part of the lasso.
	void freehandDrag(const TPointD &drawingPos, const TPointD &workingPos);

	//!Close the lasso (adds final point to m_track) and create stroke representing the lasso.
	void closeFreehand();

	//!Adds point to m_polyline vector.
	void addPointPolyline(const TPointD &drawingPos, const TPointD &workingPos);

	//!Adds final pos to \b m_polyline and closes the break (adds \b m_polyline.front() to the end of the vector)
	void closePolyline(const TPointD &drawingPos, const TPointD &workingPos);

	/*--- Enable RGBPickerTool in flipbook ---*/
	void showFlipPickedColor(const TPixel32 &pix);
};

#endif
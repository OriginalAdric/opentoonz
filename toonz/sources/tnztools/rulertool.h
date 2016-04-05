#ifndef RULERTOOL_H
#define RULERTOOL_H

#include "tools/tool.h"
#include "tproperty.h"
#include "tools/tooloptions.h"

#include <QCoreApplication>

//----------------------------------------------------------------------------------------------

class RulerTool : public TTool
{
	Q_DECLARE_TR_FUNCTIONS(RulerTool)

	enum DragMode {
		MakeNewRuler = 0,
		MoveFirstPos,
		MoveSecondPos,
		MoveRuler
	};

	TPointD m_firstPos, m_secondPos;
	TPointD m_mousePos;

	DragMode m_dragMode;

	bool m_justClicked;

	vector<RulerToolOptionsBox *> m_toolOptionsBox;

public:
	RulerTool();

	void setToolOptionsBox(RulerToolOptionsBox *toolOptionsBox);

	ToolType getToolType() const { return TTool::GenericTool; }

	void onImageChanged();

	void draw();

	void leftButtonDown(const TPointD &pos, const TMouseEvent &e);

	void leftButtonDrag(const TPointD &pos, const TMouseEvent &e);

	void leftButtonUp(const TPointD &pos, const TMouseEvent &);

	void mouseMove(const TPointD &pos, const TMouseEvent &e);

	void onActivate();

	int getCursorId() const;

	void updateToolOption();

	/*--- Returns true if mouse position is sufficiently close to the Ruler ---*/
	bool isNearRuler();

private:
	/*--- With respect to reference point, return position in which to fit
		the mouse position in 0,45,90 degrees ---*/
	TPointD getHVCoordinatedPos(TPointD p, TPointD centerPos);
};

#endif
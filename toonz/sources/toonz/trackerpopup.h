

#ifndef TRACKERPOPUP_H
#define TRACKERPOPUP_H

#include <QWidget>
#include <QThread>

#include "tlevel.h"

#include "toonzqt/dvdialog.h"

#include "toonz/txshsimplelevel.h"

// for the tracker
#include "dummyprocessor.h"
// tracker.lib
#include "ObjectTracker.h"

// forward declaration
class Tracker;
class MyThread;
class QComboBox;
class QPushButton;
class QLabel;
class QSlider;
class TXshSimpleLevel;

// forward declaration
namespace DVGui
{
class DoubleField;
class IntField;
//class LineEdit;
class CheckBox;
}

//=============================================================================
// TrackerPopup
//-----------------------------------------------------------------------------

//!This class handles displaying the "convert-To-Vector Settings" popup.
/*!It allows vectorization mode to be changed, then depending on that, to change
   the parameters to be set.*/

class TrackerPopup : public DVGui::Dialog
{
	Q_OBJECT

	DVGui::DoubleField *m_threshold;
	DVGui::DoubleField *m_sensibility;
	DVGui::CheckBox *m_activeBackground;
	DVGui::CheckBox *m_variationWindow;
	/*DVGui::LineEdit*    m_frameStart;
	DVGui::LineEdit*    m_frameEnd;*/
	QPushButton *m_trackBtn;
	DVGui::IntField *m_from;
	DVGui::IntField *m_to;

public:
#if QT_VERSION >= 0x050500
	TrackerPopup(QWidget *parent = 0, Qt::WindowFlags flags = Qt::Tool);
#else
	TrackerPopup(QWidget *parent = 0, Qt::WFlags flags = Qt::Tool);
#endif

	bool apply();

private:
	DVGui::ProgressDialog *m_progress;
	MyThread *m_myThread;
	Tracker *m_tracker;
	int m_progressBarIndex;
	bool m_stoppedProcess;
public slots:
	//! Closes the popup if vectorization completed successfully.
	void onTrack();
	//! Start tracking in a new thread
	void startNewThread();
	//! Stop tracking process after the current frame
	void stopProcessing();
};

//=============================================================================
// Tracker
//-----------------------------------------------------------------------------

class Tracker
{
	float m_threshold;
	float m_sensibility;
	int m_activeBackground;
	int m_manageOcclusion;
	int m_variationWindow;
	int m_indexFrameStart;
	int m_framesNumber;
	short m_trackerCount;
	int *m_numstart;
	//TLevel::Iterator m_currentFrameIt;
	int m_currentFrame;
	int m_num;

	short m_oldObjectId;
	short m_numobjactive;
	int m_trackerRegionIndex;
	TPointD m_rasterOrigin;

	CObjectTracker *m_pObjectTracker[30];

	TRaster32P m_raster_prev;
	TRaster32P m_raster;
	TRaster32P *m_raster_template;
	TrackerObjectsSet *m_trackerObjectsSet;
	DummyProcessor *m_processor;

	TLevelP m_level;
	TAffine m_affine;

public:
	Tracker(double threshold, double sensibility,
			int activeBackground, int manageOcclusion,
			int variationWindow, int frameStart,
			int framesNumber);
	~Tracker();
	// Return true if everything is ok
	bool trackCurrentFrame();
	int getFramesNumber() { return m_framesNumber; }
	int getStartFrameIndex() { return m_indexFrameStart; }
	QString getLastError() { return getErrorMessage(m_lastErrorCode); }
private:
	// Variable initialization
	// Return true if everything is ok
	bool setup();
	int m_lastErrorCode;
	// Returns the error message from error code given by the apply() method
	QString getErrorMessage(int errorCode);
};

class MyThread : public QThread
{
	Tracker *m_tracker;

public:
	MyThread(Tracker *tracker);
	void run();
	// Return 0 if there was no error in thread execution,
	// otherwise return tracker's error code
	int getLastError() { return m_trackerError; };
private:
	int m_trackerError;
};

#endif // TRACKERPOPUP_H

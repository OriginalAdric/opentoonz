

#ifndef CAST_VIEWER_INCLUDED
#define CAST_VIEWER_INCLUDED

#include <QSplitter>
#include <QTreeWidget>

#include "dvitemview.h"
#include "tfilepath.h"

class QLabel;
class TLevelSet;
class CastItems;

//-----------------------------------------------------------------------------

class CastTreeViewer : public QTreeWidget, public TSelection
{
	Q_OBJECT
	QTreeWidgetItem *m_dropTargetItem;
	TFilePath m_dropFilePath;

	void populateFolder(QTreeWidgetItem *folder);

public:
	CastTreeViewer(QWidget *parent = 0);
	QSize sizeHint() const;

	TFilePath getCurrentFolder() const;
	static TLevelSet *getLevelSet();

	// from TSelection
	bool isEmpty() const { return false; }
	void selectNone() {}
	void enableCommands();

protected:
	void paintEvent(QPaintEvent *);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void resizeEvent(QResizeEvent *);

public slots:
	void onItemChanged(QTreeWidgetItem *item, int column);
	void onFolderChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void onSceneNameChanged();
	void onCastFolderAdded(const TFilePath &path);

	void rebuildCastTree();
	void folderUp();
	void newFolder();
	void deleteFolder();

	void resizeToConts(void);

signals:
	void itemMovedToFolder();
};

//-----------------------------------------------------------------------------

//!The class is responsibile for visualizing the scene cast.
/*!E' Subclass of \b QSplitter and contains a tree that displays the folder \b m_treeView
	and a widget that allows you to view files \b m_sceneCastView.
	Its widgets are set via a SceneCastModel type model.*/
class CastBrowser : public QSplitter, public DvItemListModel
{
	Q_OBJECT

	CastTreeViewer *m_treeViewer;
	QLabel *m_folderName;
	DvItemViewer *m_itemViewer;

	CastItems *m_castItems;

public:
#if QT_VERSION >= 0x050500
	CastBrowser(QWidget *parent = 0, Qt::WindowFlags flags = 0);
#else
	CastBrowser(QWidget *parent = 0, Qt::WFlags flags = 0);
#endif
	~CastBrowser();

	CastItems *getCastItems() const { return m_castItems; }

	void sortByDataModel(DataType dataType, bool isDiscendent);

	int getItemCount() const;
	void refreshData();
	QVariant getItemData(int index, DataType dataType, bool isSelected = false);
	QMenu *getContextMenu(QWidget *parent, int index);
	void startDragDrop();
	bool acceptDrop(const QMimeData *data) const;
	bool drop(const QMimeData *data);

	void expose();
	void edit();
	void showFolderContents();
	void viewFile();
	void viewFileInfo();

protected:
	bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
	Qt::DropActions supportedDropActions() const;

	//void showEvent(QShowEvent*);
	//void hideEvent(QHideEvent*);

protected slots:
	void folderChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void refresh();
};

//-----------------------------------------------------------------------------

#endif

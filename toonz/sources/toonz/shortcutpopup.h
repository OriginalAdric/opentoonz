

#ifndef SHORTCUTPOPUP_H
#define SHORTCUTPOPUP_H

#include <QDialog>
#include <QTreeWidget>

#include "toonzqt/dvdialog.h"

// forward declaration
class QPushButton;
class ShortcutItem;

using namespace DVGui;

//=============================================================================
// ShortcutViewer
// --------------
// The editor for the shortcut associated with the current action
// View the shortcut and allow user to change it directly by typing the
// new keystroke sequence
// It must call removeShortcut() to clear.
//-----------------------------------------------------------------------------

class ShortcutViewer : public QWidget
{
	Q_OBJECT
	QAction *m_action;

public:
	ShortcutViewer(QWidget *parent);
	~ShortcutViewer();

protected:
	void paintEvent(QPaintEvent *);
	bool event(QEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

public slots:
	void setAction(QAction *action);
	void removeShortcut();

signals:
	void shortcutChanged();
};

//=============================================================================
// ShortcutTree
// ------------
// View all QActions (with any assigned shortcuts)
// Used to select the current QAction
//-----------------------------------------------------------------------------

class ShortcutTree : public QTreeWidget
{
	Q_OBJECT
	std::vector<ShortcutItem *> m_items;

public:
	ShortcutTree(QWidget *parent = 0);
	~ShortcutTree();

protected:
	// Adds a QAction block. commandType is a CommandType::MenubarCommandType
	void addFolder(const QString &title, int commandType, QTreeWidgetItem *folder = 0);

public slots:
	void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void onShortcutChanged();

signals:
	void actionSelected(QAction *);
};

//=============================================================================
// ShortcutPopup
// -------------
// The popup used to edit shortcuts
//-----------------------------------------------------------------------------

class ShortcutPopup : public Dialog
{
	Q_OBJECT
	QPushButton *m_removeBtn;
	ShortcutViewer *m_sViewer;
	ShortcutTree *m_list;

public:
	ShortcutPopup();
	~ShortcutPopup();
};

#endif //  SHORTCUTPOPUP_H

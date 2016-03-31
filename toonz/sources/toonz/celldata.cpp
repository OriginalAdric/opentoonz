

#include "celldata.h"
#include <assert.h>

#include "toonz/txsheet.h"
#include "toonz/txshcolumn.h"
#include "toonz/txshleveltypes.h"
#include "toonz/txshzeraryfxcolumn.h"
#include "toonz/txshzeraryfxlevel.h"
#include "toonz/tcolumnfx.h"
#include "toonz/fxdag.h"

//-----------------------------------------------------------------------------

TCellData::TCellData()
	: m_rowCount(0), m_colCount(0)
{
}

//-----------------------------------------------------------------------------

TCellData::TCellData(const TCellData *src)
	: m_cells(src->m_cells), m_rowCount(src->m_rowCount), m_colCount(src->m_colCount)
{
}

//-----------------------------------------------------------------------------

TCellData::~TCellData()
{
}

//-----------------------------------------------------------------------------

const TXshCell TCellData::getCell(int row, int col) const
{
	assert(0 <= row && row < m_rowCount);
	assert(0 <= col && col < m_colCount);
	return m_cells[row + col * m_rowCount];
}

//-----------------------------------------------------------------------------

// data <- xsh
void TCellData::setCells(TXsheet *xsh, int r0, int c0, int r1, int c1)
{
	assert(r0 <= r1 && c0 <= c1);
	m_rowCount = r1 - r0 + 1;
	m_colCount = c1 - c0 + 1;
	assert(m_rowCount > 0);
	assert(m_colCount > 0);
	m_cells.clear();

	m_cells.resize(m_rowCount * m_colCount);

	int c;
	for (c = c0; c <= c1; c++) {
		int index = c - c0;
		TXshColumn *column = xsh->getColumn(c);
		if (!column || column->isEmpty())
			continue;
		xsh->getCells(r0, c, m_rowCount, &m_cells[index * m_rowCount]);
	}
	setText("TCellData");
}

//-----------------------------------------------------------------------------

// data -> xsh
bool TCellData::getCells(TXsheet *xsh, int r0, int c0, int &r1, int &c1, bool insert, bool doZeraryClone) const
{
	int c;
	r1 = r0 + m_rowCount - 1;
	c1 = c0 + m_colCount - 1;
	bool cellSet = false;
	for (c = c0; c < c0 + m_colCount; c++) {
		TXshColumn *column = xsh->getColumn(c);
		/*- index: number of column in selection (leftmost == 0) -*/
		int index = c - c0;
		std::vector<TXshCell> cells = m_cells;
		bool isColumnEmpty = true;
		if (column) {
			isColumnEmpty = column->isEmpty();
			/*- 各セルに左上→右下で順に割り振られるIndex -*/
			/*- Index to be allocated from top-left -> lower-right in each cell -*/
			int cellIndex = index * m_rowCount;
			/*- Increment cellIndex up to the next populated cell -*/
			while (cellIndex < (index + 1) * m_rowCount && m_cells[cellIndex].isEmpty())
				++cellIndex;
			/*- End of the selected range -*/
			if ((int)m_cells.size() <= cellIndex) //Blank cells.
				return cellSet;
			/*- カラムが変更不可なら次のカラムへ -*/
			if (!canChange(column, index))
				continue;
		}
		if (doZeraryClone && isColumnEmpty)
			cloneZeraryFx(index, cells);

		// Enter the cells
		if (insert)
			xsh->insertCells(r0, c, m_rowCount);
		cellSet = xsh->setCells(r0, c, m_rowCount, &cells[index * m_rowCount]);
	}
	return cellSet;
}

//-----------------------------------------------------------------------------
/*-- Check that all columns within the selected width of TCellData starting at c0 can be changed*/
bool TCellData::canChange(TXsheet *xsh, int c0) const
{
	int c;
	for (c = c0; c < c0 + m_colCount; c++) {
		int index = c - c0;
		TXshColumn *column = xsh->getColumn(c);
		if (!canChange(column, index))
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------------

bool TCellData::canChange(TXshColumn *column, int index) const
{
	/*- No column or empty column (editable) -*/
	if (!column || column->isEmpty())
		return true;
	/*- False when the column is locked（non-editable） -*/
	if (column->isLocked())
		return false;
	/*-- Check that all cells in which to paste can be set --*/
	TXshCellColumn *cellColumn = column->getCellColumn();
	assert(cellColumn);
	int i;
	for (i = index * m_rowCount; i < (index * m_rowCount) + m_rowCount; i++)
		//for(i = 0; i < m_cells.size(); i++)
		if (!cellColumn->canSetCell(m_cells[i]))
			return false;
	return true;
}

//-----------------------------------------------------------------------------

void TCellData::cloneZeraryFx(int index, std::vector<TXshCell> &cells) const
{
	int firstNotEmptyIndex = index * m_rowCount;
	while (firstNotEmptyIndex < (index + 1) * m_rowCount && m_cells[firstNotEmptyIndex].isEmpty())
		firstNotEmptyIndex++;
	if (firstNotEmptyIndex >= (index + 1) * m_rowCount)
		return;
	TXshZeraryFxLevel *fxLevel = m_cells[firstNotEmptyIndex].m_level->getZeraryFxLevel();
	if (!fxLevel)
		return;
	TXshZeraryFxColumn *fxColumn = fxLevel->getColumn();
	assert(fxColumn);
	TFx *zeraryFx = fxColumn->getZeraryColumnFx()->getZeraryFx();
	if (zeraryFx) {
		wstring fxName = zeraryFx->getName();
		TFx *newZeraryFx = zeraryFx->clone(false);
		fxColumn->getXsheet()->getFxDag()->assignUniqueId(newZeraryFx);
		newZeraryFx->setName(fxName);
		newZeraryFx->linkParams(zeraryFx);
		TXshZeraryFxLevel *newFxLevel = new TXshZeraryFxLevel();
		TXshZeraryFxColumn *newFxColumn = new TXshZeraryFxColumn(0);
		newFxColumn->getZeraryColumnFx()->setZeraryFx(newZeraryFx);
		newFxLevel->setColumn(newFxColumn);
		cells.clear();
		int r;
		for (r = firstNotEmptyIndex; r < (index + 1) * m_rowCount; r++)
			cells.push_back(TXshCell(newFxLevel, m_cells[r].getFrameId()));
	}
}

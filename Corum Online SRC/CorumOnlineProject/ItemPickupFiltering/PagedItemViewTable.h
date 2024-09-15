#pragma once
#include "ItemInfoView.h"
#include <vector>

class CItem;

class PagedItemViewTable {
public:
	PagedItemViewTable(Rect frame, std::vector<CItem*> allItems);
	void updateFilter(const char* nameFilter);
	void render();
private: 
	std::vector<CItem*> _allItems;
	std::vector<CItem*> _currentDisplayedItems;
	std::vector<std::vector<ItemInfoView*>> _viewsTable;
	Rect _frame;
	int _numberOfRows;
	int _numberOfColumns;
	int _currentTopRowIndex;

	CItem* getCurrentItemForDisplayedCell(int row, int column);
	void updateDisplayedRowsWithCurrentItems();

};


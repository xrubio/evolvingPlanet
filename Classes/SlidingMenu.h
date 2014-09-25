
#ifndef __SLIDING_MENU_GRID_H
#define __SLIDING_MENU_GRID_H
/*
*Addon class for cocos2d-x framework (http://www.cocos2d-x.org) 
*Based on class Brandon Reynolds  http://brandonreynolds.com/blog/2011/01/09/cocos2d-sliding-menu-grid/,
*and port sources Mr.  K pop(http://www.cocos2d-x.org/boards/6/topics/9803) 
* Make by Starfair studio (starfair@inbox.ru)
* v1.0 29.03.2012 Add Label (CCLabelTTF *)for any pages (see metod  setPageLabel())
* v1.1 30.03.2012 Tag number saved in each CCItemMenu * in CCMenuItemsArray not rewrite inside SlidingMenuGrid (see metod initWithArray()) 
*				  and change logic to set items on pages (first added set on first page)
* v1.2 19.03.2012 Fixed a bug where a locked item indicated by the touch as the chosen.
				  Fixed a bug page counter by Mr.  K pop (http://www.cocos2d-x.org/boards/18/topics/9931?r=10275#message-10275	)
				  Added the ability to be used as a descriptor page of any object pointer CCNode *
* v2.0 19.08.2014 Version for COCOS2d-X 3.X
*/

#include "cocos2d.h"

USING_NS_CC;

typedef Vector<MenuItem*> MenuItemsArray;

class SlidingMenuGrid:public Layer
{
protected:
	void buildGrid(int cols, int rows);
	void buildGridVertical(int cols, int rows);
	void moveToCurrentPage();

	virtual bool TouchBegan(Touch *pTouch, Event *pEvent);
	virtual void TouchMoved(Touch *pTouch, Event *pEvent);
	virtual void TouchEnded(Touch *pTouch, Event *pEvent);
	virtual void TouchCancelled(Touch *pTouch,Event *pEvent);

	Point GetPositionOfCurrentPage();
	Point GetPositionOfCurrentPageWithOffset(float offset);
	MenuItem* GetItemWithinTouch(Touch* touch);

	Menu::State state; // State of our menu grid. (Eg. waiting, tracking touch, cancelled, etc)
	MenuItem *selectedItem; // Menu item that was selected/active.

	Point padding; // Padding in between menu items. 
	Point menuOrigin; // Origin position of the entire menu grid.
	Point touchOrigin; // Where the touch action began.
	Point touchStop; // Where the touch action stopped.

	Size winSize;

	int iPageCount; // Number of pages in this grid.
	int iCurrentPage; // Current page of menu items being viewed.

	bool bMoving; // Is the grid currently moving?
	bool bSwipeOnlyOnMenu; // Causes swiping functionality to only work when siping on top of the menu items instead of entire screen.
	bool bVerticalPaging; // Disabled by default. Allows for pages to be scrolled vertically instead of horizontal.

	float fMoveDelta; // Distance from origin of touch and current frame.
	float fMoveDeadZone; // Amount they need to slide the grid in order to move to a new page.
	float fAnimSpeed; // 0.0-1.0 value determining how slow/fast to animate the paging.

	int iMenuRows;
	Menu * pMenu;
	bool initWithArray(MenuItemsArray* items, int cols, int rows, Point pos , Point pad, bool vertical);
public:
	SlidingMenuGrid();
	~SlidingMenuGrid();
	virtual bool init();
	virtual void onExit();
	static SlidingMenuGrid* createWithArray(MenuItemsArray* items, int cols, int rows, Point pos , Point pad, bool vertical=false);
	float GetSwipeDeadZone();
	void SetSwipeDeadZone(float fValue);
	bool IsVerticallyPaged();
	void SetVerticalPaging(bool bValue);
	int getPageCount(){return iPageCount;}
	
	void gotoPage(int iPage=0);
	void setPageLabel(int,Node *,bool bTop=true);//Add label on the page
};
#endif
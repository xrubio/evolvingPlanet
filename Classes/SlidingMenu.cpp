#include "SlidingMenu.h"

SlidingMenuGrid::SlidingMenuGrid()
{
}

SlidingMenuGrid::~SlidingMenuGrid()
{
}

void SlidingMenuGrid::onExit()
{
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

bool SlidingMenuGrid::init()
{
    if (!Layer::init()) {
        return false;
    }
    winSize = Director::getInstance()->getWinSize();
    return true;
}

SlidingMenuGrid* SlidingMenuGrid::createWithArray(MenuItemsArray* items, int cols, int rows, Point pos, Point pad, bool vertical)
{
    SlidingMenuGrid* slidingMenu = new SlidingMenuGrid();
    if (slidingMenu && slidingMenu->initWithArray(items, cols, rows, pos, pad, vertical)) {
        slidingMenu->autorelease();
        return slidingMenu;
    }
    CC_SAFE_DELETE(slidingMenu);
    return nullptr;
}

bool SlidingMenuGrid::initWithArray(MenuItemsArray* items, int cols, int rows, Point pos, Point pad, bool vertical)
{
    if (!Layer::init() && items == nullptr) {
        return false;
    }

    winSize = Director::getInstance()->getWinSize();

    selectedItem = NULL;

    auto listener = EventListenerTouchOneByOne::create();
    // When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event, preventing other listeners from using it.
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(SlidingMenuGrid::TouchBegan, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SlidingMenuGrid::TouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(SlidingMenuGrid::TouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(SlidingMenuGrid::TouchMoved, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    pMenu = Menu::create();
    addChild(pMenu, 0);
    pMenu->setPosition(Point(0, 0));

    for (int i = 0; i < items->size(); i++) {
        auto getItem = items->at(i);
        pMenu->addChild(getItem, 1, getItem->getTag());
    }
    padding = pad;
    iCurrentPage = 0;
    bMoving = false;
    iMenuRows = rows;
    menuOrigin = pos;
    fMoveDeadZone = 10;
    bVerticalPaging = vertical;
    fAnimSpeed = 1;
    state = Menu::State::WAITING;
    (bVerticalPaging) ? this->buildGridVertical(cols, rows) : this->buildGrid(cols, rows);
    return true;
}

void SlidingMenuGrid::buildGrid(int cols, int rows)
{
    int col = 0, row = 0;
    auto child = pMenu->getChildren();
    Point position;
    iPageCount = 0;
    float posX = menuOrigin.x - cols * padding.x * 0.5f + padding.x * 0.5f;
    float posY = winSize.height - menuOrigin.y + rows * padding.y * 0.5f - padding.y * 0.5f;
    for (int i = 0; i < child.size(); i++) {
        auto item = dynamic_cast<MenuItem*>(child.at(i));
        position.x = posX + col * padding.x + iPageCount * winSize.width;
        position.y = posY - row * padding.y;
        item->setPosition(position);
        ++col;
        if (col == cols) {
            col = 0;
            ++row;
            if (row == rows) {
                iPageCount++;
                col = 0;
                row = 0;
            }
        }
    }
    if (child.size() > rows * cols * iPageCount) //   <-- add code for FIX (Mr.  K pop)
    {
        iPageCount++;
    }
}

void SlidingMenuGrid::buildGridVertical(int cols, int rows)
{
    int col = 0, row = 0;
    auto child = pMenu->getChildren();
    Point position;
    iPageCount = 0;
    float posX = menuOrigin.x - cols * padding.x * 0.5f + padding.x * 0.5f;
    float posY = winSize.height - menuOrigin.y + rows * padding.y * 0.5f - padding.y * 0.5f;
    for (int i = 0; i < child.size(); i++) {
        auto item = dynamic_cast<MenuItem*>(child.at(i));
        position.x = posX + col * padding.x;
        position.y = posY - row * padding.y - (iPageCount * winSize.height);
        item->setPosition(position);
        ++col;
        if (col == cols) {
            col = 0;
            ++row;
            if (row == rows) {
                iPageCount++;
                col = 0;
                row = 0;
            }
        }
    }
    if (child.size() > rows * cols * iPageCount) //   <-- add code for FIX (Mr.  K pop)
    {
        iPageCount++;
    }
}

MenuItem* SlidingMenuGrid::GetItemWithinTouch(Touch* touch)
{
    Point touchLocation = Director::getInstance()->convertToGL(touch->getLocationInView());

    // Parse our menu items and see if our touch exists within one.
    auto child = pMenu->getChildren();
    Node* item;
    for (int i = 0; i < child.size(); i++) {

        item = child.at(i);
        auto getItem = dynamic_cast<MenuItem*>(item);

        Point local = getItem->convertToNodeSpace(touchLocation);
        Rect r = getItem->rect();
        r.origin = Point::ZERO; // If the touch was within this item. Return the item.
        if (r.containsPoint(local)) {
            return getItem;
        }
    }
    // Didn't touch an item.
    return NULL;
}

bool SlidingMenuGrid::TouchBegan(Touch* touch, Event* event)
{
    touchOrigin = touch->getLocationInView();
    // If we weren't in "waiting" state bail out.
    if (state == Menu::State::WAITING) {
        // Activate the menu item if we are touching one.
        if (!bMoving) {
            selectedItem = GetItemWithinTouch(touch);
            if (selectedItem) {
                if (selectedItem->isEnabled())
                    selectedItem->selected();
            }
        }
        state = Menu::State::TRACKING_TOUCH;
    }
    return true;
}

void SlidingMenuGrid::TouchCancelled(Touch* touch, Event* event)
{
    if (selectedItem) {
        selectedItem->unselected();
        selectedItem = NULL;
        state = Menu::State::WAITING;
    }
}

void SlidingMenuGrid::TouchMoved(Touch* touch, Event* event)
{
    if (GetItemWithinTouch(touch) == NULL && selectedItem) {
        //Touch Cancelled
        if (selectedItem->isEnabled()) {
            selectedItem->unselected();
        }
        selectedItem = NULL;
        state = Menu::State::WAITING;
        return;
    }

    if (GetItemWithinTouch(touch) != NULL && selectedItem) {
        return;
    }
    // Calculate the current touch point during the move.
    touchStop = touch->getLocationInView();
    fMoveDelta = (bVerticalPaging) ? (touchOrigin.y - touchStop.y) : (touchStop.x - touchOrigin.x); // Set our position.
    if (!bMoving) {
        bMoving = true;
        return;
    }
    setPosition(GetPositionOfCurrentPageWithOffset(fMoveDelta));
}

void SlidingMenuGrid::TouchEnded(Touch* touch, Event* event)
{
    if (bMoving) {
        bMoving = false;
        {
            // Do we have multiple pages?
            if (iPageCount > 1 && (fMoveDeadZone < abs(fMoveDelta))) {
                // Are we going forward or backward?
                bool bForward = (fMoveDelta > 0) ? true : false;

                // Do we have a page available?
                if (bVerticalPaging) {
                    if (bForward && (iPageCount > iCurrentPage + 1)) {
                        // Increment currently active page.
                        iCurrentPage++;
                    } else if (!bForward && (iCurrentPage > 0)) {
                        // Decrement currently active page.
                        iCurrentPage--;
                    }
                } else {
                    if (!bForward && (iPageCount > iCurrentPage + 1)) {
                        // Increment currently active page.
                        iCurrentPage++;
                    } else if (bForward && (iCurrentPage > 0)) {
                        // Decrement currently active page.
                        iCurrentPage--;
                    }
                }

                // Start sliding towards the current page.
                moveToCurrentPage();
            } else {
                if (iPageCount == 1) {
                    moveToCurrentPage();
                }

                if (selectedItem) {
                    selectedItem->unselected();
                    selectedItem->activate();
                }
            }
        }
    } else {
        //Only touch
        if (selectedItem) {
            if (selectedItem->isEnabled()) {
                selectedItem->unselected();
                selectedItem->activate();
            }
        }
    }
    state = Menu::State::WAITING;
}

void SlidingMenuGrid::moveToCurrentPage()
{
    auto action = MoveTo::create(fAnimSpeed * 0.3f, GetPositionOfCurrentPage());
    runAction(action);
}

Point SlidingMenuGrid::GetPositionOfCurrentPage()
{
    Point position;
    if (bVerticalPaging)
        position = Point(0, iCurrentPage * winSize.height);
    else
        position = Point(-(iCurrentPage)*winSize.width, 0);
    return position;
}

Point SlidingMenuGrid::GetPositionOfCurrentPageWithOffset(float offset)
{
    Point position = GetPositionOfCurrentPage();
    if (bVerticalPaging)
        position.y += offset;
    else
        position.x += offset;
    return position;
}

// Returns the swiping dead zone.
float SlidingMenuGrid::GetSwipeDeadZone()
{
    return fMoveDeadZone;
}

void SlidingMenuGrid::SetSwipeDeadZone(float fValue)
{
    fMoveDeadZone = fValue;
}

// Returns wheather or not vertical paging is enabled.
bool SlidingMenuGrid::IsVerticallyPaged()
{
    return bVerticalPaging;
}

// Sets the vertical paging flag.
void SlidingMenuGrid::SetVerticalPaging(bool bValue)
{
    bVerticalPaging = bValue;
}

void SlidingMenuGrid::gotoPage(int iPage)
{
    int iSetPage = iPage;
    if (iPage <= 0)
        iSetPage = 1;
    else if (iPage > iPageCount)
        iSetPage = iPageCount;

    iCurrentPage = iSetPage - 1;
    moveToCurrentPage();
}

void SlidingMenuGrid::setPageLabel(int iPage, Node* pLabel, bool bTop)
{
    if (!pLabel)
        return;
    addChild(pLabel, 10);
    int iSetPage = iPage;
    if (iPage <= 0)
        iSetPage = 1;
    else if (iPage > iPageCount)
        iSetPage = iPageCount;

    Size labelSize = pLabel->getContentSize();
    Point pt;
    if (bVerticalPaging)
        pt.x = menuOrigin.x;
    else
        pt.x = menuOrigin.x + (iSetPage - 1) * winSize.width;
    if (bTop) {
        if (bVerticalPaging)
            pt.y = winSize.height - menuOrigin.y + iMenuRows * padding.y * 0.5 + labelSize.height / 2 - winSize.height * (iSetPage - 1);
        else
            pt.y = winSize.height - menuOrigin.y + iMenuRows * padding.y * 0.5 + labelSize.height / 2;
    } else {
        if (bVerticalPaging)
            pt.y = winSize.height - menuOrigin.y - iMenuRows * padding.y * 0.5 - labelSize.height / 2 - winSize.height * (iSetPage - 1);
        else
            pt.y = winSize.height - menuOrigin.y - iMenuRows * padding.y * 0.5 - labelSize.height / 2;
    }

    pLabel->setPosition(pt);
}

#include "MenuManager.h"
#include <string.h>

MenuManager::MenuManager(const MenuItem *root, int itemCount)
{
  menuRoot = root;
  rootMenuItemCount = itemCount;
  
  currentMenu = menuRoot;
  currentMenuItemCount = rootMenuItemCount;
  currentMenuItemIndexPos = 0;
  menuStackCount = 0;
}

// ---------------------------------------------------
void MenuManager::reset()
{
  currentMenu = menuRoot;
  currentMenuItemCount = rootMenuItemCount;
  currentMenuItemIndexPos = 0;
  menuStackCount = 0;
}

// ---------------------------------------------------
char *MenuManager::getParentItemName(char *buf)
{
  *buf = 0;
  
  MenuStackItem *msi = peekMenuItemOnStack();

  if (msi != 0)
  {
    strcpy(buf, msi->menu[msi->itemIndexPos].name);
  }
  return buf;
}

// ---------------------------------------------------
char *MenuManager::getItemName(char * buf, int idx)
{
  return strcpy(buf, currentMenu[idx].name);
}

// ---------------------------------------------------
int MenuManager::itemHasChildren(int idx)
{
  return (currentMenu[idx].childItemCount) > 0;
}

// ---------------------------------------------------
char *MenuManager::getCurrentItemName(char *buf)
{
  return strcpy(buf, currentMenu[currentMenuItemIndexPos].name);
}

// ---------------------------------------------------
const int MenuManager::getCurrentItemCmdId()
{
  return currentMenu[currentMenuItemIndexPos].id;
}

// ---------------------------------------------------
const int MenuManager::getMenuItemCount()
{
  return currentMenuItemCount;
}

// ---------------------------------------------------
const MenuItem *MenuManager::getMenuItem()
{
  return currentMenu;
}
// ---------------------------------------------------
const int MenuManager::moveToItem(int itemNo)
{
  if (itemNo < (currentMenuItemCount))
  {
    currentMenuItemIndexPos = itemNo;
    return 1;
  }
  return 0;
}

// ---------------------------------------------------
const int MenuManager::getCurrentItemIndex()
{
  return currentMenuItemIndexPos;
}

// ---------------------------------------------------
int MenuManager::moveToNextItem()
{
  if (currentMenuItemIndexPos < (currentMenuItemCount -1))
  {
    currentMenuItemIndexPos++;
    return 1;
  }
  return 0;
}

// ---------------------------------------------------
int MenuManager::moveToPreviousItem()
{
  if (currentMenuItemIndexPos > 0)
  {
    currentMenuItemIndexPos--;
    return 1;
  }
  return 0;
}

// ---------------------------------------------------
const int MenuManager::currentItemHasChildren()
{
  return currentMenu[currentMenuItemIndexPos].childItemCount > 0;
}

// ---------------------------------------------------
const int MenuManager::currentMenuHasParent()
{
  return stackHasItems();
}

// ---------------------------------------------------
void MenuManager::descendToChildMenu()
{
  if (currentItemHasChildren())
  {
    pushMenuOnStack(currentMenu, currentMenuItemIndexPos, currentMenuItemCount);
    
    currentMenuItemCount = currentMenu[currentMenuItemIndexPos].childItemCount;
    currentMenu = currentMenu[currentMenuItemIndexPos].childMenu;
    currentMenuItemIndexPos = 0;
  }
}

// ---------------------------------------------------
void MenuManager::ascendToParentMenu()
{
  if (currentMenuHasParent())
  {
    MenuStackItem *msi = popMenuItemFromStack();

    currentMenu = msi->menu;
    currentMenuItemCount = msi->itemCount;
    currentMenuItemIndexPos = msi->itemIndexPos;
  }
}


// ---------------------------------------------------
int MenuManager::handleNavigation(int (*getNavAction)(), void (*refreshDisplay)(int))
{
  int menuMode = MENU_REMAIN;
  int action = getNavAction();

  if (action == MENU_ITEM_SELECT || action == MENU_BACK)      // enter menu item, or sub menu, or ascend to parent, or cancel.
  {
    if (getCurrentItemCmdId() == 0 || action == MENU_BACK)
    {
      if (!currentMenuHasParent())
      {
        menuMode = MENU_EXIT;
        reset();
      }
      else
      {
        ascendToParentMenu();
        refreshDisplay(REFRESH_ASCEND);
      }
    }
    else if (currentItemHasChildren())
    {
      descendToChildMenu();
      refreshDisplay(REFRESH_DESCEND);
    }
    else
    {
      menuMode = MENU_INVOKE_ITEM;
    }
  }
  else if (action == MENU_ITEM_PREV) // move prev
  {
    if (moveToPreviousItem())
    {
      refreshDisplay(REFRESH_MOVE_PREV);
    }
  }
  else if (action == MENU_ITEM_NEXT) // move next
  {
    if (moveToNextItem())
    {
      refreshDisplay(REFRESH_MOVE_NEXT);
    }
  }

  return menuMode;
}


// ---------------------------------------------------
const int MenuManager::stackHasItems()
{
  return menuStackCount > 0;
}

// ---------------------------------------------------
void MenuManager::pushMenuOnStack(const MenuItem *menu, int indexPos, int itemCount)
{
  if (menuStackCount < (sizeof (menuStack) / sizeof(MenuStackItem)))
  {
    menuStack[menuStackCount].itemIndexPos = indexPos;
    menuStack[menuStackCount].itemCount = itemCount;
    menuStack[menuStackCount].menu = menu;
    menuStackCount++;
  }
}

// ---------------------------------------------------
MenuStackItem *MenuManager::popMenuItemFromStack()
{
  MenuStackItem *menuStackItem = 0;
  
  if (stackHasItems())
  {
    menuStackCount--;
    menuStackItem = &(menuStack[menuStackCount]);
  }

  return menuStackItem;
}

// ---------------------------------------------------
MenuStackItem *MenuManager::peekMenuItemOnStack()
{
  MenuStackItem *menuStackItem = 0;
  
  if (stackHasItems())
  {
    menuStackItem = &(menuStack[menuStackCount-1]);
  }
  return menuStackItem;
}

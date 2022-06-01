#ifndef MenuManager_h_
#define MenuManager_h_

#define MENU_MANAGER_1_0

typedef struct MenuItem {
  int id;
  const char *name;
  const MenuItem *childMenu;
  int childItemCount; 
} MenuItem;

#define menuCount(m)  (sizeof(m) / sizeof(MenuItem))

typedef struct MenuStackItem {
  int itemIndexPos;
  int itemCount;
  const MenuItem *menu;
} MenuStackItem;

enum MenuMode         // Values returned by MenuManager::handleNavigation().
{
  MENU_REMAIN = 0,    // user is navigating menu.
  MENU_EXIT,          // user exits menu navigation.
  MENU_INVOKE_ITEM    // user has selected menu item.
};

enum MenuNavAction    // Used by call back function to map user input to these values.
{
  MENU_ITEM_PREV = 1,
  MENU_ITEM_NEXT,
  MENU_ITEM_SELECT,
  MENU_BACK
};

enum DisplayRefreshMode   // Used by call back function to instruct display refresh according to menu navigation.
{
  REFRESH_MOVE_PREV,  // user has navigated to previous menu item.
  REFRESH_MOVE_NEXT,  // user has navigated to next menu item.
  REFRESH_ASCEND,     // user has navigated to parent menu.
  REFRESH_DESCEND     // user has navigated to child menu.
};

class MenuManager
{
  public:
    MenuManager (const MenuItem *root, int itemCount);

    // Resets the menu so it points to the first item of the root menu.
    void reset();

    // Gets the menu item name of the parent. Caller needs to first check if currentMenuHasParent().
    char *getParentItemName(char *buf);
    
    // Gets the menu item name, given item index position
    char *getItemName(char *buf, int idx);

    // Returns true if specified menu item has child menu items.
    int itemHasChildren(int idx);
    
    // gets the current menu item name.
    char *getCurrentItemName(char *buf);
    
    // Gets the current menu item command id.
    const int getCurrentItemCmdId();

    // Returns the number of items in the current menu.
    const int getMenuItemCount();
    // Gets the current Menu;
    const MenuItem *getMenuItem();
    
    // Moves to specified menu item. Returns true if successful.
    const int moveToItem(int itemIndex);
    // Gets the current menu item index.
    const int getCurrentItemIndex();
    
    // Moves to next menu item. Returns true if there was an item to move to.
    int moveToNextItem();
    // Moves to previous menu item. Returns true if there was an item to move to.
    int moveToPreviousItem();

    // Returns true if current menu item has child menu items.
    const int currentItemHasChildren();
    // Returns true if current menu has a parent menu.
    const int currentMenuHasParent();

    // Decends to current item's child menu.
    void descendToChildMenu();
    // Ascends to current item's parent menu.
    void ascendToParentMenu();


    // Handles menu navigation. Returns MenuMode enum value.
    int handleNavigation(
      int (*getNavAction)(),        // call back function that returns MenuNavAction enum value.
      void (*refreshDisplay)(int)   // call back function that updates the menu display
                                              // using paramter of DisplayRefreshMode enum value.
      );
  
  private:
    const MenuItem *menuRoot;
    int rootMenuItemCount;

    const MenuItem *currentMenu;
    int currentMenuItemCount;
    int currentMenuItemIndexPos;
    
    MenuStackItem menuStack[5];
    int menuStackCount;
    
    const int stackHasItems();
    void pushMenuOnStack(const MenuItem *menu, int indexPos, int itemCount);
    MenuStackItem *popMenuItemFromStack();
    MenuStackItem *peekMenuItemOnStack();
    
};

#endif

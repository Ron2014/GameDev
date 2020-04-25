#include "UIWnd.h"
#include <map>
#include <string>

class UIWndTerrainList: public UIWnd
{
protected:
    char **m_filenames;
    ITEM **m_items;
    MENU *m_pMenu;
    int m_count;

    virtual void OnResize();
    virtual void OnUpdate();

public:
    UIWndTerrainList(/* args */);
    virtual ~UIWndTerrainList();

    virtual void Clean();
};
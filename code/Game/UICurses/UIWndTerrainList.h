#include "UIWnd.h"
#include <map>
#include <string>

class UIWndTerrainList: public UIWnd
{
protected:
    char **m_filenames;
    int m_iCount;

#if FUTURE_POSIX
    ITEM **m_items;
    MENU *m_pMenu;
#else
    int m_iChoice;
    int m_iMenuLen;
#endif

    virtual void OnResize();
    virtual void OnUpdate();

public:
    UIWndTerrainList(/* args */);
    virtual ~UIWndTerrainList();

    virtual void Clean();
};
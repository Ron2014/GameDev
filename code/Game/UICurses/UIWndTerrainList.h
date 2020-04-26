#include "UIWnd.h"

class UIWndTerrainList: public UIWnd
{
protected:
    char **m_filenames;
    int m_iCount;
    int m_iChoice;
    int m_iMenuLen;

    virtual void OnResize();
    virtual void OnUpdate();

public:
    UIWndTerrainList(/* args */);
    virtual ~UIWndTerrainList();

    virtual void Clean();
};
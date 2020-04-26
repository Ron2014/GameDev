#include "UIWnd.h"

class UIWndTerrainPref: public UIWnd
{
protected:
    int m_iLine;
    int m_iChoice;

    char m_sInput[128];
    int m_iInputPos;

    virtual void OnUpdate();
    virtual void OnResize();

public:
    static const UIWnd::TYPE type;

    UIWndTerrainPref(/* args */);
    virtual ~UIWndTerrainPref();

    virtual void OnDestroy();
};
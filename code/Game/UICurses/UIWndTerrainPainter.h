#include "UIWndMotion.h"

class UIWndTerrainPainter: public UIWndMotion
{
protected:
    int m_iLine;
    virtual void OnUpdate();
    virtual void OnResize();
    virtual void OnInit();

public:
    static const UIWnd::TYPE type;

    UIWndTerrainPainter(/* args */);
    virtual ~UIWndTerrainPainter();

    virtual void OnDestroy();
};
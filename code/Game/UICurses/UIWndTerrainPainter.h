#include "UIWndMotion.h"

class UIWndTerrainPainter: public UIWndMotion
{
protected:
    /* data */
    virtual void OnUpdate();
    virtual void OnResize();

public:
    static const UIWnd::TYPE type;

    UIWndTerrainPainter(/* args */);
    virtual ~UIWndTerrainPainter();
};
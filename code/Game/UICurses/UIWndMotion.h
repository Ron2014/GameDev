#include "UIWnd.h"
#include "Global.h"

class UIWndMotion: public UIWnd
{
protected:
    /* data */
    virtual void OnUpdate();
    virtual void OnResize();

public:
    static const UIWnd::TYPE type;
    
    UIWndMotion(/* args */);
    virtual ~UIWndMotion();
};
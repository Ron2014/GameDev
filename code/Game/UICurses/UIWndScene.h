#include <set>
#include "UIWnd.h"
#include "Global.h"
#include "Terrain/TerrainConfig.h"

class UIWndScene: public UIWnd
{
protected:
    void DrawTerrain(TerrainConfig *);
    void DrawEntity(TerrainConfig *, std::set<int> &);

    virtual bool NeedUpdate();
    virtual void OnUpdate();

public:
    static const UIWnd::TYPE type;

    UIWndScene(/* args */);
    ~UIWndScene();
};
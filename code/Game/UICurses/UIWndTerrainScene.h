#include <set>
#include "UIWnd.h"
#include "Terrain/TerrainConfig.h"

class UIWndTerrainScene: public UIWnd
{
protected:
    void DrawTerrain(TerrainConfig *);
    void DrawEntity(TerrainConfig *, std::set<int> &);

    virtual bool NeedUpdate();
    virtual void OnUpdate();

public:
    static const UIWnd::TYPE type;

    UIWndTerrainScene(/* args */);
    virtual ~UIWndTerrainScene();
};
#include "Global.h"
#include "UIWndTerrainScene.h"

const UIWnd::TYPE UIWndTerrainScene::type = UIWnd::TYPE::terrain_scene;

UIWndTerrainScene::UIWndTerrainScene(/* args */):UIWnd()
{
    m_Anchor = UIWnd::ANCHOR::MIDDLE_CENTER;

    TerrainGrid::type_chtype[TerrainGrid::Walkable] = ' ';
    TerrainGrid::type_chtype[TerrainGrid::Walkable_Wall] = ACS_CKBOARD;
    TerrainGrid::type_chtype[TerrainGrid::Walkable_Grass] = ACS_STERLING;

    gRefreshWorld = true;
}

UIWndTerrainScene::~UIWndTerrainScene()
{
}

bool UIWndTerrainScene::NeedUpdate(){
    if (!gRefreshWorld) return false;
    gRefreshWorld = false;
    return true;
}

void UIWndTerrainScene::OnUpdate() {
    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    // printf("UIWndTerrainScene::OnUpdate\n");
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    DrawTerrain(terrainCfg);
    DrawEntity(terrainCfg, world->GetEntityIDs());
}

void UIWndTerrainScene::DrawTerrain(TerrainConfig * terrainCfg) {
    // render terrain of this world
    int linePixel = terrainCfg->GetLinePixel();
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    // screen pixel
    m_width = gridCol;
    m_height = gridRow;
    
    Resize();

    // draw only grid
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            TerrainGrid::TYPE gt = terrainCfg->GetGridType(j, gridRow-i-1);
            chtype ch = TerrainGrid::GetChtype(gt);
            if (ch) mvwaddch(m_pWnd, i+CURSES_BOADER, j+CURSES_BOADER, ch);
        }
    }
}

void UIWndTerrainScene::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
    int linePixel = terrainCfg->GetLinePixel();
    double lineLength = terrainCfg->GetLineLength();
    
    // get entity from world
    std::set<int>::iterator it = entity_ids.begin();
	attron(A_BOLD);

    while(it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            ComponentLocation *cl = e->GetComponent<ComponentLocation>();

            if (cl) {
                // get position & heading
                Vector3D &position = cl->vPosition;
                int xgrid = int(position.x / lineLength);
                int ygrid = m_height - int(position.z  / lineLength) - 1;
                mvwaddch(m_pWnd, ygrid+CURSES_BOADER, xgrid+CURSES_BOADER, ACS_DIAMOND);
            }
        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }

        it++;
    }

	attroff(A_BOLD);
}
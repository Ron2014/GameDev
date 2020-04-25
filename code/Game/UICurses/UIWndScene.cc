#include "UIWndScene.h"

const UIWnd::TYPE UIWndScene::type = UIWnd::TYPE::scene;

UIWndScene::UIWndScene(/* args */):UIWnd()
{
    m_Anchor = UIWnd::ANCHOR::MIDDLE_CENTER;

    TerrainGrid::type_chtype[TerrainGrid::Walkable_Wall] = ACS_CKBOARD;
    TerrainGrid::type_chtype[TerrainGrid::Walkable_Grass] = ACS_PLUS;
}

UIWndScene::~UIWndScene()
{
}

bool UIWndScene::NeedUpdate(){
    if (!gRefreshWorld) return false;
    gRefreshWorld = false;
    return true;
}

void UIWndScene::OnUpdate() {
    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    DrawTerrain(terrainCfg);
    DrawEntity(terrainCfg, world->GetEntityIDs());
}

void UIWndScene::DrawTerrain(TerrainConfig * terrainCfg) {
    // render terrain of this world
    int linePixel = terrainCfg->GetLinePixel();
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    // screen pixel
    m_width = linePixel * gridCol;
    m_height = linePixel * gridRow;
    
    Resize();

    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++) {
            TerrainGrid::TYPE gt = terrainCfg->GetGridType(j/linePixel, gridRow-(i/linePixel)-1);
            chtype ch = TerrainGrid::GetChtype(gt);
            if (ch) mvwaddch(m_pWnd, i+CURSES_BOADER, j+CURSES_BOADER, ch);
        }
    }
}

void UIWndScene::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
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
                Vector3D &heading = cl->vHeading;

                int xpixel = int(position.x * linePixel / lineLength);
                int ypixel = m_height - int(position.z * linePixel / lineLength) - 1;

                if (heading.x > 0)
                    mvwaddch(m_pWnd, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_LTEE);
                else if(heading.z > 0)
                    mvwaddch(m_pWnd, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_BTEE);
                else if(heading.x < 0)
                    mvwaddch(m_pWnd, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_RTEE);
                else if(heading.z < 0)
                    mvwaddch(m_pWnd, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_TTEE);
                else
                    mvwaddch(m_pWnd, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_BTEE);

                ComponentHealthPoint *ch = e->GetComponent<ComponentHealthPoint>();
                if (ch) {
                    int maxHP = ch->iMaxHP;
                    int curHP = ch->iCurHP;

                    // render
                }
            }
        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }

        it++;
    }

	attroff(A_BOLD);
}
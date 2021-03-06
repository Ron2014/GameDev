#include <vector>
#include "SystemWorldRender.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentHealthPoint.h"

/**
 * world -> entity & component
 * ComponentLocation
 * ComponentHealthPoint
*/

int SystemWorldRender::refresh_count = 0;

#include "Core/Util/NcursesUtil.h"

static WINDOW *scene_win = nullptr;

SystemWorldRender::SystemWorldRender():m_iWorldID(0),System() {
    printf("SystemWorldRender::SystemWorldRender\n");
	initscr();
    curs_set(FALSE);

    TerrainGrid::type_chtype[TerrainGrid::None] = ' ';
    TerrainGrid::type_chtype[TerrainGrid::Wall] = ACS_CKBOARD;
    TerrainGrid::type_chtype[TerrainGrid::Grass] = ACS_PLUS;
    TerrainGrid::type_chtype[TerrainGrid::Border] = ACS_BLOCK;
}

SystemWorldRender::~SystemWorldRender() {
    endwin();
}

void SystemWorldRender::DrawTerrain(TerrainConfig * terrainCfg) {
    // render terrain of this world
    int linePixel = terrainCfg->GetLinePixel();
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    // screen pixel
    int swidth = linePixel * gridCol;
    int sheight = linePixel * gridRow;

    // ±2 for border
    int starty = (LINES - sheight - 2*CURSES_BOADER) / 2;
    int startx = (COLS - swidth - 2*CURSES_BOADER) / 2;

    if(scene_win) destroy_win(scene_win);
    scene_win = create_newwin(sheight + 2*CURSES_BOADER, swidth + 2*CURSES_BOADER, starty, startx);

    WINDOW *cur_win = scene_win;
    for (int i=0; i<sheight; i++) {
        for (int j=0; j<swidth; j++) {
            TerrainGrid::TYPE gt = terrainCfg->GetGridType(j/linePixel, gridRow-(i/linePixel)-1);
            // switch(gt) {
            //     case TerrainGrid::Walkable:
            //         break;
            //     case TerrainGrid::Walkable_Wall:
            //         mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_CKBOARD);
            //         break;
            //     case TerrainGrid::Walkable_Grass:
            //         mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_PLUS);
            //         break;
            // }
            chtype ch = TerrainGrid::GetChtype(gt);
            if (ch) mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ch);
        }
    }
    wrefresh(cur_win);
}

void SystemWorldRender::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
    int linePixel = terrainCfg->GetLinePixel();
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();
    double lineLength = terrainCfg->GetLineLength();

    // screen pixel
    int swidth = linePixel * gridCol;
    int sheight = linePixel * gridRow;
    
    WINDOW *cur_win = scene_win;
    
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
                int ypixel = sheight - int(position.z * linePixel / lineLength) - 1;

                if (heading.x > 0)
                    mvwaddch(cur_win, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_LTEE);
                else if(heading.z > 0)
                    mvwaddch(cur_win, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_BTEE);
                else if(heading.x < 0)
                    mvwaddch(cur_win, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_RTEE);
                else if(heading.z < 0)
                    mvwaddch(cur_win, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_TTEE);
                else
                    mvwaddch(cur_win, ypixel+CURSES_BOADER, xpixel+CURSES_BOADER, ACS_BTEE);

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
    wrefresh(cur_win);
}

void SystemWorldRender::Update()
{
    if (!gRefreshWorld) return;
    gRefreshWorld = false;
    
    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }
    
	mvprintw(LINES - 1, 0, "refresh count %d, LINES: %d, COLS: %d", refresh_count++, LINES, COLS);
    wrefresh(stdscr);

    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    DrawTerrain(terrainCfg);
    DrawEntity(terrainCfg, world->GetEntityIDs());
}

// #endif
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
}

SystemWorldRender::~SystemWorldRender() {
    endwin();
}

void SystemWorldRender::DrawTerrain(TerrainConfig * terrainCfg) {
    // render terrain of this world
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    // ±2 for border
    int starty = (LINES - gridRow - 2*CURSES_BOADER) / 2;
    int startx = (COLS - gridCol - 2*CURSES_BOADER) / 2;

    if(scene_win) destroy_win(scene_win);
    scene_win = create_newwin(gridRow + 2*CURSES_BOADER, gridCol + 2*CURSES_BOADER, starty, startx);

    WINDOW *cur_win = scene_win;

    for (int i=0; i<gridRow; i++) {
        for (int j=0; j<gridCol; j++) {
            grid_type gt = terrainCfg->GetGridType(j, gridRow-i-1);
            switch(gt) {
                case Walkable:
                    // mvwaddch(cur_win, starty+i, startx+j, ACS_BULLET);
                    break;
                case Walkable_Non:
                    mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_CKBOARD);
                    break;
                case StartPoint:
                    mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_DIAMOND);
                    break;
                case Flyable:
                    mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_LANTERN);
                    break;
                case EndPoint:
                    mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_PLUS);
                    break;
                case HeroPoint:
                    mvwaddch(cur_win, i+CURSES_BOADER, j+CURSES_BOADER, ACS_PLMINUS);
                    break;
                default:
                    break;
            }
        }
    }
    wrefresh(cur_win);
}

void SystemWorldRender::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();
    
    WINDOW *cur_win = scene_win;
    
    // get entity from world
    std::set<int>::iterator it = entity_ids.begin();
	attron(A_BOLD);

    while( it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            if (c) {
                // get position & heading
                Vector3D &position = ((ComponentLocation *)c)->vPosition;
                Vector3D &heading = ((ComponentLocation *)c)->vHeading;

                int pRow = gridRow-terrainCfg->Pos2GridRow(position)-1;
                int pCol = terrainCfg->Pos2GridCol(position);

                if (heading.x > 0)
                    mvwaddch(cur_win, pRow+CURSES_BOADER, pCol+CURSES_BOADER, ACS_LTEE);
                else if(heading.z > 0)
                    mvwaddch(cur_win, pRow+CURSES_BOADER, pCol+CURSES_BOADER, ACS_BTEE);
                else if(heading.x < 0)
                    mvwaddch(cur_win, pRow+CURSES_BOADER, pCol+CURSES_BOADER, ACS_RTEE);
                else if(heading.z < 0)
                    mvwaddch(cur_win, pRow+CURSES_BOADER, pCol+CURSES_BOADER, ACS_TTEE);
                else
                    mvwaddch(cur_win, pRow+CURSES_BOADER, pCol+CURSES_BOADER, ACS_BTEE);

                c = e->GetComponent(Component::health_point);
                if (c) {
                    int maxHP = ((ComponentHealthPoint *)c)->iMaxHP;
                    int curHP = ((ComponentHealthPoint *)c)->iCurHP;

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
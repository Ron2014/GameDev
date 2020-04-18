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

#if FUTURE_WINDOWS

// directX
// openGL
// win32

SystemWorldRender::SystemWorldRender():m_iWorldID(0),System() {
}

SystemWorldRender::~SystemWorldRender() {
}

void SystemWorldRender::DrawTerrain(TerrainConfig *terrainCfg) {

}

void SystemWorldRender::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
    
}

void SystemWorldRender::Update()
{
    system("cls");

    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }

    // render terrain of this world
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    // terrainCfg->DumpData();

    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    std::vector< std::vector<int> > entity_board(gridRow);
    for (int i=0; i<gridRow; i++){
        for (int j=0; j<gridCol; j++)
            entity_board[i].push_back(-1);
    }

    // get entity from world
    std::set<int> entity_ids = world->GetEntityIDs();
    std::set<int>::iterator it = entity_ids.begin();

    while( it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            if (c) {
                // get position & heading
                Vector3D position = ((ComponentLocation *)c)->vPosition;
                Vector3D heading = ((ComponentLocation *)c)->vHeading;

                int pRow = terrainCfg->Pos2GridRow(position);
                int pCol = terrainCfg->Pos2GridCol(position);

                entity_board[pRow][pCol] = e->GetID();
                // render

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

    for (int i=0; i<gridRow; i++) {
        for (int j=0; j<gridCol; j++) {
            if (entity_board[gridRow-i-1][j] >= 0) {
                printf("-%d-|", entity_board[gridRow-i-1][j]);
            } else 
                printf(" %d |", terrainCfg->GetGridType(j, gridRow-i-1));
        }
        printf("\n");
    }
}

#else

#include <curses.h>

SystemWorldRender::SystemWorldRender():m_iWorldID(0),System() {
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	srand(time(NULL));
}

SystemWorldRender::~SystemWorldRender() {
    endwin();
}

void SystemWorldRender::DrawTerrain(TerrainConfig * terrainCfg) {
    // render terrain of this world
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    int starty = (LINES - gridRow) / 2;
    int startx = (COLS - gridCol) / 2;

    for (int i=0; i<gridRow; i++) {
        for (int j=0; j<gridCol; j++) {
            grid_type gt = terrainCfg->GetGridType(j, gridRow-i-1);
            switch(gt) {
                case Walkable:
                    // mvwaddch(stdscr, starty+i, startx+j, ACS_BULLET);
                    break;
                case Walkable_Non:
                    mvwaddch(stdscr, starty+i, startx+j, ACS_CKBOARD);
                    break;
                case StartPoint:
                    mvwaddch(stdscr, starty+i, startx+j, ACS_DIAMOND);
                    break;
                case Flyable:
                    mvwaddch(stdscr, starty+i, startx+j, ACS_LANTERN);
                    break;
                case EndPoint:
                    mvwaddch(stdscr, starty+i, startx+j, ACS_PLUS);
                    break;
                case HeroPoint:
                    mvwaddch(stdscr, starty+i, startx+j, ACS_PLMINUS);
                    break;
                default:
                    break;
            }
        }
    }
    wrefresh(stdscr);
}

void SystemWorldRender::DrawEntity(TerrainConfig *terrainCfg, std::set<int> &entity_ids) {
    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    int starty = (LINES - gridRow) / 2;
    int startx = (COLS - gridCol) / 2;
    
    // get entity from world
    std::set<int>::iterator it = entity_ids.begin();

    while( it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            if (c) {
                // get position & heading
                Vector3D position = ((ComponentLocation *)c)->vPosition;
                Vector3D heading = ((ComponentLocation *)c)->vHeading;

                int pRow = gridRow-terrainCfg->Pos2GridRow(position)-1;
                int pCol = terrainCfg->Pos2GridCol(position);

                if (heading.x > 0)
                    mvwaddch(stdscr,starty+pRow,startx+pCol,ACS_LTEE);
                else if(heading.z > 0)
                    mvwaddch(stdscr,starty+pRow,startx+pCol,ACS_BTEE);
                else if(heading.x < 0)
                    mvwaddch(stdscr,starty+pRow,startx+pCol,ACS_RTEE);
                else if(heading.z < 0)
                    mvwaddch(stdscr,starty+pRow,startx+pCol,ACS_TTEE);
                else
                    mvwaddch(stdscr,starty+pRow,startx+pCol,ACS_BTEE);

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
    wrefresh(stdscr);
}

void SystemWorldRender::Update()
{
	clear();

    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }
    
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    DrawTerrain(terrainCfg);
    DrawEntity(terrainCfg, world->GetEntityIDs());

	mvprintw(LINES - 1, 0, "refresh count %d, LINES: %d, COLS: %d", refresh_count++, LINES, COLS);
    refresh();
}

#endif
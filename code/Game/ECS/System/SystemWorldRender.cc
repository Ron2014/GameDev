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
// PDCurses

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

#define BOADER 1

static WINDOW *global_win = nullptr;

static WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/
	return local_win;
}

static void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */

	// wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	// /* The parameters taken are 
	//  * 1. win: the window on which to operate
	//  * 2. ls: character to be used for the left side of the window 
	//  * 3. rs: character to be used for the right side of the window 
	//  * 4. ts: character to be used for the top side of the window 
	//  * 5. bs: character to be used for the bottom side of the window 
	//  * 6. tl: character to be used for the top left corner of the window 
	//  * 7. tr: character to be used for the top right corner of the window 
	//  * 8. bl: character to be used for the bottom left corner of the window 
	//  * 9. br: character to be used for the bottom right corner of the window
	//  */
	// wrefresh(local_win);

    wclear(local_win);
	delwin(local_win);
}

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

    // ±2 for border
    int starty = (LINES - gridRow - 2*BOADER) / 2;
    int startx = (COLS - gridCol - 2*BOADER) / 2;

    if(global_win) destroy_win(global_win);
    global_win = create_newwin(gridRow + 2*BOADER, gridCol + 2*BOADER, starty, startx);

    WINDOW *cur_win = global_win;

    for (int i=0; i<gridRow; i++) {
        for (int j=0; j<gridCol; j++) {
            grid_type gt = terrainCfg->GetGridType(j, gridRow-i-1);
            switch(gt) {
                case Walkable:
                    // mvwaddch(cur_win, starty+i, startx+j, ACS_BULLET);
                    break;
                case Walkable_Non:
                    mvwaddch(cur_win, i+BOADER, j+BOADER, ACS_CKBOARD);
                    break;
                case StartPoint:
                    mvwaddch(cur_win, i+BOADER, j+BOADER, ACS_DIAMOND);
                    break;
                case Flyable:
                    mvwaddch(cur_win, i+BOADER, j+BOADER, ACS_LANTERN);
                    break;
                case EndPoint:
                    mvwaddch(cur_win, i+BOADER, j+BOADER, ACS_PLUS);
                    break;
                case HeroPoint:
                    mvwaddch(cur_win, i+BOADER, j+BOADER, ACS_PLMINUS);
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
    
    WINDOW *cur_win = global_win;
    
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
                    mvwaddch(cur_win, pRow+BOADER, pCol+BOADER, ACS_LTEE);
                else if(heading.z > 0)
                    mvwaddch(cur_win, pRow+BOADER, pCol+BOADER, ACS_BTEE);
                else if(heading.x < 0)
                    mvwaddch(cur_win, pRow+BOADER, pCol+BOADER, ACS_RTEE);
                else if(heading.z < 0)
                    mvwaddch(cur_win, pRow+BOADER, pCol+BOADER, ACS_TTEE);
                else
                    mvwaddch(cur_win, pRow+BOADER, pCol+BOADER, ACS_BTEE);

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

    wrefresh(cur_win);
}

void SystemWorldRender::Update()
{
    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }
    
	mvprintw(LINES - 1, 0, "refresh count %d, LINES: %d, COLS: %d", refresh_count++, LINES, COLS);
    refresh();

    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    DrawTerrain(terrainCfg);
    DrawEntity(terrainCfg, world->GetEntityIDs());
}

#endif
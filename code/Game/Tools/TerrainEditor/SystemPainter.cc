#include "SystemPainter.h"
#include "ECS/Component/ComponentLocation.h"
#include "ECS/Component/ComponentMoving.h"
#include "ComponentPainter.h"

using namespace Future;

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
 * ComponentPainter
*/

#include "Core/Util/NcursesUtil.h"

static WINDOW *top_win = nullptr;
WINDOW *my_wins[3];
PANEL  *my_panels[3];

SystemPainter::SystemPainter():m_iPlayerID(0), System() {
    printf("SystemPainter::SystemPainter\n");
    SetType(System::painter);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);

    // file list: menus
	my_wins[0] = create_newwin(10+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);

    // map properties
    my_wins[1] = create_newwin(10+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);

    // paint info
    my_wins[2] = create_newwin(10+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);
	
	/* Attach a panel to each window */ 	/* Order is bottom up */
	my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
	my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
	my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */

	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[2]);
	set_panel_userptr(my_panels[2], my_panels[0]);

	/* Update the stacking order. 2nd panel will be on top */
	update_panels();

	/* Show it on the screen */
	doupdate();

    top_win = create_newwin(10+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);
    mvwprintw(top_win, 1, 1, "Move:w-  a-  s-  d- ");
    mvwaddch(top_win, 1, 8, ACS_UARROW);
    mvwaddch(top_win, 1, 12, ACS_LARROW);
    mvwaddch(top_win, 1, 16, ACS_DARROW);
    mvwaddch(top_win, 1, 20, ACS_RARROW);
    mvwprintw(top_win, 2, 1, "Exit:F1");
}

void SystemPainter::Update() {
    int ch = getch();
    if(ch == KEY_F(1)) {
        gExitGame = true;
        return;
    }

    unsigned int gt = 0;
    Vector3D pos;

    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        ComponentMoving *c = e->GetComponent<ComponentMoving>();
        ComponentLocation *l = e->GetComponent<ComponentLocation>();
        ComponentPainter *p = e->GetComponent<ComponentPainter>();

        pos = l->vPosition;
        gt = p->pen;

        Vector3D v;
        switch (ch) {
            case 'w':
                v = Vector3D(0.0, 0.0, c->iMaxSpeed);
                break;
            case 'a':
                v = Vector3D(-c->iMaxSpeed, 0.0, 0.0);
                break;
            case 's':
                v = Vector3D(0.0, 0.0, -c->iMaxSpeed);
                break;
            case 'd':
                v = Vector3D(c->iMaxSpeed, 0.0, 0.0);
                break;
            case '\t': // switch
                gt++;
                if (gt == (unsigned int)TerrainGrid::TYPE::TypeCount) gt = 0;
                p->pen = (TerrainGrid::TYPE)gt;
                break;
            case ' ':  // paint
                if (gt) {

                }
                break;
            case KEY_BACKSPACE: // erase
                break;
            default:
                break;
        }
        c->vVelocity = v;
        if (!v.isZero()) l->vHeading = v;

    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }

    if (gRefreshControl || ch != ERR) {
        gRefreshControl = false;
        box(top_win, 0, 0);
        mvwprintw(top_win, 3, 1, "CUR GRID: ", pos.x, pos.z);
        mvwaddch(top_win, 3, 10, TerrainGrid::GetChtype(gt));
        mvwprintw(top_win, 4, 1, "POS:%.02f, %.02f", pos.x, pos.z);
        mvwprintw(top_win, 5, 1, "Pressed:%c(%d)", ch, ch);
        wrefresh(top_win);
    }
}
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

static WINDOW *control_win = nullptr;

SystemPainter::SystemPainter():m_iPlayerID(0), System() {
    printf("SystemPainter::SystemPainter\n");
    SetType(System::painter);

    control_win = create_newwin(10+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);
    mvwprintw(control_win, 1, 1, "Move:w-  a-  s-  d- ");
    mvwaddch(control_win, 1, 8, ACS_UARROW);
    mvwaddch(control_win, 1, 12, ACS_LARROW);
    mvwaddch(control_win, 1, 16, ACS_DARROW);
    mvwaddch(control_win, 1, 20, ACS_RARROW);
    mvwprintw(control_win, 2, 1, "Exit:F1");
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
        ComponentMoving *c = (ComponentMoving *)e->GetComponent(Component::moving);
        ComponentLocation *l = (ComponentLocation *)e->GetComponent(Component::location);
        ComponentPainter *p = (ComponentPainter *)e->GetComponent(Component::painter);

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
                if (gt == (unsigned int)TerrainGrid::Type::TypeCount) gt = 0;
                p->pen = (TerrainGrid::Type)gt;
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
        box(control_win, 0, 0);
        mvwprintw(control_win, 3, 1, "CUR GRID: ", pos.x, pos.z);
        mvwaddch(control_win, 3, 10, grid_chtype[gt]);
        mvwprintw(control_win, 4, 1, "POS:%.02f, %.02f", pos.x, pos.z);
        mvwprintw(control_win, 5, 1, "Pressed:%c(%d)", ch, ch);
        wrefresh(control_win);
    }
}
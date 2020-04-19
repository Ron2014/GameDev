#include "SystemMotionSteer.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"

using namespace Future;

/**
 * entity & component
 * ComponentLocation
 * ComponentMoving
*/

#if FUTURE_WINDOWS

SystemMotionSteer::SystemMotionSteer():m_iPlayerID(0), System() {
    SetType(System::motion_steer);
}

void SystemMotionSteer::Update() {
    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)
    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }
}

#else

#include "Core/Util/NcursesUtil.h"

static WINDOW *control_win = nullptr;

SystemMotionSteer::SystemMotionSteer():m_iPlayerID(0), System() {
    printf("SystemMotionSteer::SystemMotionSteer\n");
    SetType(System::motion_steer);
    
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    // timeout(CURSES_TIMEOUT);
    control_win = create_newwin(1+2*CURSES_BOADER, 20+2*CURSES_BOADER, 0, 0);
    wtimeout(control_win, CURSES_TIMEOUT);
}

void SystemMotionSteer::Update() {
    int ch = wgetch(control_win); /* Waits for TIME_OUT milliseconds */
	// if(ch == ERR)
	// 	return;

    if(ch == KEY_F(1)) {
        gExitGame = true;
        return;
    }

    Vector3D pos;
    Entity *e = gEntityMgr.GetMember(m_iPlayerID);
    if (e) {
        // signal from keyboard
        // signal from joystick(xbox controller)

        ComponentMoving *c = (ComponentMoving *)e->GetComponent(Component::moving);
        ComponentLocation *l = (ComponentLocation *)e->GetComponent(Component::location);
        pos = l->vPosition;

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
            default:
                break;
        }
        c->vVelocity = v;
        if (!v.isZero()) l->vHeading = v;

    } else {
        Log::Error("Render Error: entity %d not exist!", m_iPlayerID);
    }

    mvwprintw(control_win, 1, 1, "%c pos(%.02f, %.02f)", ch, pos.x, pos.z);
    wrefresh(control_win);
}

#endif
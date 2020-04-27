#include "UIWndMotion.h"

const UIWnd::TYPE UIWndMotion::type = UIWnd::TYPE::control;

UIWndMotion::UIWndMotion(/* args */):UIWnd()
{
    m_Anchor = UIWnd::ANCHOR::LEFT_TOP;
    m_width = 30;
    m_height = 10;
    m_keyWnd = true;
}

UIWndMotion::~UIWndMotion()
{
}

void UIWndMotion::OnResize() {
    mvwprintw(m_pWnd, 1, 1, "Move:w-  a-  s-  d- ");
    mvwaddch(m_pWnd, 1, 8, ACS_UARROW);
    mvwaddch(m_pWnd, 1, 12, ACS_LARROW);
    mvwaddch(m_pWnd, 1, 16, ACS_DARROW);
    mvwaddch(m_pWnd, 1, 20, ACS_RARROW);
    mvwprintw(m_pWnd, 2, 1, "Exit:F1");

    gRefreshControl = true;
}

void UIWndMotion::OnUpdate() {
    Entity *e = gEntityMgr.GetMember(gContollingEntityID);
    if (!e) {
        Log::Error("Render Error: entity %d not exist!", gContollingEntityID);
        return;
    }

    ComponentMoving *c = e->GetComponent<ComponentMoving>();
    ComponentLocation *l = e->GetComponent<ComponentLocation>();
    Vector3D pos = l->vPosition;

    int ch;
    Vector3D v;
    if ((ch = getch()) != ERR) {
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
            case KEY_ESC:
                gExitGame = true;
                break;
            default:
                break;
        }
        c->vVelocity = v;
        if (!v.isZero()) l->vHeading = v;
        gRefreshControl = true;
    }

    if (gRefreshControl) {
        gRefreshControl = false;
        mvwprintw(m_pWnd, 3, 1, "POS:%.02f, %.02f", pos.x, pos.z);
        mvwprintw(m_pWnd, 4, 1, "Pressed:%c(%d)", ch, ch);
    }
}

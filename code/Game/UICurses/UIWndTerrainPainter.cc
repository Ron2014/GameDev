#include "UIWndTerrainPainter.h"
#include "UIMgr.h"
#include "UIWndTerrainList.h"

const UIWnd::TYPE UIWndTerrainPainter::type = UIWnd::TYPE::terrain_painter;

UIWndTerrainPainter::UIWndTerrainPainter(/* args */):UIWndMotion()
{
    m_keyWnd = true;
}

UIWndTerrainPainter::~UIWndTerrainPainter()
{
}

void UIWndTerrainPainter::OnResize() {
    mvwprintw(m_pWnd, 1, 1, "Move:w-  a-  s-  d- ");
    mvwaddch(m_pWnd, 1, 8, ACS_UARROW);
    mvwaddch(m_pWnd, 1, 12, ACS_LARROW);
    mvwaddch(m_pWnd, 1, 16, ACS_DARROW);
    mvwaddch(m_pWnd, 1, 20, ACS_RARROW);

    mvwprintw(m_pWnd, 2, 1, "Switch:<TAB> %d", '\t');
    mvwprintw(m_pWnd, 3, 1, "Paint:<SPACE> %d", ' ');
    mvwprintw(m_pWnd, 4, 1, "Erase:<DEL> %d", KEY_DC);
    
    mvwprintw(m_pWnd, 5, 1, "Save:F10");
    mvwprintw(m_pWnd, 6, 1, "ESC:F1");
    
	keypad(m_pWnd, TRUE);
    wtimeout(m_pWnd, CURSES_TIMEOUT);

    gRefreshControl = true;
}

void UIWndTerrainPainter::OnUpdate() {
    Entity *e = gEntityMgr.GetMember(gContollingEntityID);
    if (!e) {
        Log::Error("Render Error: entity %d not exist!", gContollingEntityID);
        return;
    }

    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    // printf("UIWndTerrainPainter::OnUpdate\n");
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    ComponentMoving *c = e->GetComponent<ComponentMoving>();
    ComponentLocation *l = e->GetComponent<ComponentLocation>();
    ComponentPainter *p = e->GetComponent<ComponentPainter>();
    Vector3D pos = l->vPosition;

    int ch;
    Vector3D v;
    if ((ch = wgetch(m_pWnd)) != ERR) {
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
            case '\t':
            {
                unsigned int val_brush = (unsigned int)p->pen;
                val_brush++;
                if (val_brush == TerrainGrid::TYPE::TypeCount)
                    val_brush = TerrainGrid::TYPE::Walkable;
                p->pen = (TerrainGrid::TYPE)val_brush;
            }
                break;
            case ' ':
            {
                terrainCfg->SetPointType(pos, p->pen);
                gRefreshWorld = true;
            }
                break;
            case KEY_DC:
            {
                terrainCfg->SetPointType(pos, TerrainGrid::TYPE::Walkable);
                gRefreshWorld = true;
            }
                break;
            case KEY_F(10):
                terrainCfg->SaveData();
                break;
            case KEY_ESC:
            {
                // go back
                UIMgr::Instance()->CreateWnd<UIWndTerrainList>();
                Destroy();
                break;
            }
            default:
                break;
        }
        gRefreshControl = true;
    }
    c->vVelocity = v;
    if (!v.isZero()) l->vHeading = v;

    if (gRefreshControl) {
        gRefreshControl = false;
        mvwprintw(m_pWnd, 7, 1, "Current Grid: ");
        mvwaddch(m_pWnd, 7, 14, TerrainGrid::GetChtype(p->pen));

        mvwprintw(m_pWnd, 8, 1, "POS:%.02f, %.02f", pos.x, pos.z);
        mvwprintw(m_pWnd, 9, 1, "Pressed:%c(%d)", ch, ch);
    }
}

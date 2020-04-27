#include "UIWndTerrainPainter.h"
#include "UIMgr.h"
#include "UIWndTerrainPref.h"
#include "UIWndTerrainList.h"
#include "UIWndTerrainScene.h"
#include "World/WorldTerrainEditor.h"
#include "Entity/EntityPainter.h"

const UIWnd::TYPE UIWndTerrainPainter::type = UIWnd::TYPE::terrain_painter;

UIWndTerrainPainter::UIWndTerrainPainter(/* args */):UIWndMotion()
{
    m_height = LINES - CURSES_BOADER * 2;
    m_keyWnd = true;
}

UIWndTerrainPainter::~UIWndTerrainPainter()
{
}

void UIWndTerrainPainter::OnInit() {
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("UIWndTerrainPainter::OnInit Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    Entity *entity = gEntityMgr.GetMember(gContollingEntityID);
    if (!entity) {
        entity = new EntityPainter();
        gContollingEntityID = entity->GetID();
    }
    world->Enter(entity);
    gRefreshWorld = true;
}

void UIWndTerrainPainter::OnDestroy() {
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("UIWndTerrainPainter::OnDestroy Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    Entity *entity = gEntityMgr.GetMember(gContollingEntityID);
    if (entity) world->Leave(entity);
    gRefreshWorld = true;
}

void UIWndTerrainPainter::OnResize() {
    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("OnResize Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    TerrainConfig *terrainCfg = world->GetTerrainConfig();

    SetTitle("Terrain Info");
    
    m_iLine = 3;
    mvwprintw(m_pWnd, m_iLine++, 1, "Name: %s", terrainCfg->GetName().c_str());
    mvwprintw(m_pWnd, m_iLine++, 1, "Stage: %s", terrainCfg->GetStage().c_str());
    mvwprintw(m_pWnd, m_iLine++, 1, "Level: %s", terrainCfg->GetLevel().c_str());
    mvwprintw(m_pWnd, m_iLine++, 1, "Grid Row: %d", terrainCfg->GetGridRow());
    mvwprintw(m_pWnd, m_iLine++, 1, "Grid Column: %d", terrainCfg->GetGridCol());
    mvwprintw(m_pWnd, m_iLine++, 1, "Line Length: %0.2f", terrainCfg->GetLineLength());
    mvwprintw(m_pWnd, m_iLine++, 1, "Line Pixel: %d", terrainCfg->GetLinePixel());

    SetTitle("Control Info", ++m_iLine);

    m_iLine += 2;
    mvwprintw(m_pWnd, m_iLine++, 1, "Move:w-  a-  s-  d- ");
    mvwaddch(m_pWnd, m_iLine-1, 8, ACS_UARROW);
    mvwaddch(m_pWnd, m_iLine-1, 12, ACS_LARROW);
    mvwaddch(m_pWnd, m_iLine-1, 16, ACS_DARROW);
    mvwaddch(m_pWnd, m_iLine-1, 20, ACS_RARROW);

    mvwprintw(m_pWnd, m_iLine++, 1, "Switch:<TAB>");
    mvwprintw(m_pWnd, m_iLine++, 1, "Paint:<SPACE>");
    mvwprintw(m_pWnd, m_iLine++, 1, "Edit:<F1>");
    mvwprintw(m_pWnd, m_iLine++, 1, "Save:<F10>");
    mvwprintw(m_pWnd, m_iLine++, 1, "Exit:<ESC>");
    mvwprintw(m_pWnd, m_iLine++, 1, "Current Grid: ");

    gRefreshControl = true;
}

void UIWndTerrainPainter::OnUpdate() {
    Entity *e = gEntityMgr.GetMember(gContollingEntityID);
    if (!e) {
        Log::Error("OnUpdate Error: entity %d not exist!", gContollingEntityID);
        return;
    }

    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("OnUpdate Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    ComponentMoving *c = e->GetComponent<ComponentMoving>();
    ComponentLocation *l = e->GetComponent<ComponentLocation>();
    ComponentPainter *p = e->GetComponent<ComponentPainter>();
    Vector3D pos = l->vPosition;

    int ch = wgetch(m_pWnd);
    Vector3D v;
    switch (ch) {
        case 'w':
        case KEY_UP:
            v = Vector3D(0.0, 0.0, c->iMaxSpeed);
            break;
        case 'a':
        case KEY_LEFT:
            v = Vector3D(-c->iMaxSpeed, 0.0, 0.0);
            break;
        case 's':
        case KEY_DOWN:
            v = Vector3D(0.0, 0.0, -c->iMaxSpeed);
            break;
        case 'd':
        case KEY_RIGHT:
            v = Vector3D(c->iMaxSpeed, 0.0, 0.0);
            break;
        case '\t':
        {
            unsigned int val_brush = (unsigned int)p->pen;
            val_brush++;
            if (val_brush == TerrainGrid::TYPE::TypeCount)
                val_brush = TerrainGrid::TYPE::None;
            p->pen = (TerrainGrid::TYPE)val_brush;
            mvwaddch(m_pWnd, m_iLine-1, 14, TerrainGrid::GetChtype(p->pen));
        }
            break;
        case ' ':
        {
            terrainCfg->SetPointType(pos, p->pen);
            gRefreshWorld = true;
        }
            break;
        case KEY_F(10):
        {
            terrainCfg->SaveData();
            mvwprintw(m_pWnd, m_height, 1, "%s saved", terrainCfg->GetName().c_str());
        }
            break;
        case KEY_F(1):
        {
            TerrainConfig *tmpTerrain = world->LoadTempTerrain(terrainCfg->GetName() + ".edit");
            *tmpTerrain = *terrainCfg;
            UIMgr::Instance()->CreateWnd<UIWndTerrainPref>();
            Destroy();
        }
            break;
        case KEY_ESC:
        {
            // go back
            UIMgr::Instance()->CreateWnd<UIWndTerrainList>();
            UIMgr::Instance()->DestroyWnd<UIWndTerrainScene>();
            Destroy();
        }
            break;
        default:
            break;
    }

    c->vVelocity = v;
    if (!v.isZero()) l->vHeading = v;

    if (gRefreshControl) {
        gRefreshControl = false;
        mvwprintw(m_pWnd, m_iLine, 1, "POS:%.02f, %.02f", pos.x, pos.z);
    }
}

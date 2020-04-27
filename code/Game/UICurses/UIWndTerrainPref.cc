#include "UIWndTerrainPref.h"
#include "UIWndTerrainPainter.h"
#include "UIWndTerrainList.h"
#include "UIMgr.h"
#include "Global.h"
#include "Core/IO/Path.h"

const UIWnd::TYPE UIWndTerrainPref::type = UIWnd::TYPE::terrain_pref;

UIWndTerrainPref::UIWndTerrainPref(/* args */):
    UIWnd(),
    m_iChoice(0),
    m_iInputPos(0)
{
    m_Anchor = UIWnd::ANCHOR::LEFT_TOP;
    m_width = 30;
    m_height = LINES - CURSES_BOADER * 2;
    m_sInput[m_iInputPos] = '\0';
    m_keyWnd = true;

    curs_set(2);
}

UIWndTerrainPref::~UIWndTerrainPref()
{
}

void UIWndTerrainPref::OnDestroy() {
    curs_set(FALSE);
}

void UIWndTerrainPref::OnResize() {
    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("OnResize Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    TerrainConfig *terrainCfg = world->GetTerrainConfig();

    SetTitle("Terrain Info");
    
    int line = 3;
    mvwprintw(m_pWnd, line++, 1, "Name:        %s", terrainCfg->GetName().c_str());
    mvwprintw(m_pWnd, line++, 1, "Stage:       %s", terrainCfg->GetStage().c_str());
    mvwprintw(m_pWnd, line++, 1, "Level:       %s", terrainCfg->GetLevel().c_str());
    mvwprintw(m_pWnd, line++, 1, "Grid Row:    %d", terrainCfg->GetGridRow());
    mvwprintw(m_pWnd, line++, 1, "Grid Column: %d", terrainCfg->GetGridCol());
    mvwprintw(m_pWnd, line++, 1, "Line Length: %0.2f", terrainCfg->GetLineLength());
    mvwprintw(m_pWnd, line++, 1, "Line Pixel:  %d", terrainCfg->GetLinePixel());

    SetTitle("Control Info", ++line);

    line += 2;
    mvwprintw(m_pWnd, line++, 1, "Move:UP-  DOWN- ");
    mvwaddch(m_pWnd, line-1, 9, ACS_UARROW);
    mvwaddch(m_pWnd, line-1, 16, ACS_DARROW);

    mvwprintw(m_pWnd, line++, 1, "Confirm:<F10>");
    mvwprintw(m_pWnd, line++, 1, "Exit:<ESC>");

    m_iLine = 7;
    wmove(m_pWnd, 3+m_iChoice, 13);
}

void UIWndTerrainPref::OnUpdate() {        
    // which world need render
    World *world = gWorldMgr.GetMember(gWatchingWorldID);
    if (!world){
        Log::Error("OnResize Error: world %d not exist!", gWatchingWorldID);
        return;
    }

    int maxInputLen = m_width-14;
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    if (m_isDirty) {
        m_isDirty = false;

        char* fmt = (char *)malloc( (m_width+1) * sizeof(char));
        int line = 3;

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Name:        %%-%ds", maxInputLen);
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetName().c_str());
        // mvwprintw(m_pWnd, line++, 1, "Name:        %s", terrainCfg->GetName().c_str());

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Stage:       %%-%ds", maxInputLen);        
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetStage().c_str());
        // mvwprintw(m_pWnd, line++, 1, "Stage:       %s", terrainCfg->GetStage().c_str());

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Level:       %%-%ds", maxInputLen);        
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetLevel().c_str());
        // mvwprintw(m_pWnd, line++, 1, "Level:       %s", terrainCfg->GetLevel().c_str());

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Grid Row:    %%-%dd", maxInputLen);        
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetGridRow());
        // mvwprintw(m_pWnd, line++, 1, "Grid Row:    %d", terrainCfg->GetGridRow());

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Grid Column: %%-%dd", maxInputLen);        
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetGridCol());
        // mvwprintw(m_pWnd, line++, 1, "Grid Column: %d", terrainCfg->GetGridCol());
        

        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Line Length: %%-%d.2f", maxInputLen);        
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetLineLength());
        // mvwprintw(m_pWnd, line++, 1, "Line Length: %.2f", terrainCfg->GetLineLength());


        memset(fmt, 0, m_width+1);
        snprintf(fmt, m_width, "Line Pixel:  %%-%dd", maxInputLen);
        mvwprintw(m_pWnd, line++, 1, fmt, terrainCfg->GetLinePixel());
        // mvwprintw(m_pWnd, line++, 1, "Line Pixel:  %d", terrainCfg->GetLinePixel());

        wattron(m_pWnd, A_STANDOUT);
        mvwprintw(m_pWnd, 3+m_iChoice, 14, "%s", m_sInput);
        wattroff(m_pWnd, A_STANDOUT);

        free(fmt);
    }

    int ch = wgetch(m_pWnd);
    switch(ch) {
        case KEY_UP:
        {
            m_iInputPos = 0;
            m_sInput[m_iInputPos] = '\0';
            m_iChoice = (m_iChoice - 1 + m_iLine) % m_iLine;
            m_isDirty = true;
        }
            break;
        case KEY_DOWN:
        {
            m_iInputPos = 0;
            m_sInput[m_iInputPos] = '\0';
            m_iChoice = (m_iChoice + 1) % m_iLine;
            m_isDirty = true;
        }
            break;
        case KEY_DC:
        case '\b':
        {
            if (m_iInputPos>0){
                m_sInput[--m_iInputPos] = '\0';
                m_isDirty = true;
            }
        }
            break;
        case '\n':
        {
            // finish input
            switch(m_iChoice) {
                case 0:
                    terrainCfg->SetName(m_sInput);
                    break;
                case 1:
                    terrainCfg->SetStage(m_sInput);
                    break;
                case 2:
                    terrainCfg->SetLevel(m_sInput);
                    break;
                case 3:
                    terrainCfg->SetGridRow(atoi(m_sInput));
                    break;
                case 4:
                    terrainCfg->SetGridCol(atoi(m_sInput));
                    break;
                case 5:
                    terrainCfg->SetLineLength(atof(m_sInput));
                    break;
                case 6:
                    terrainCfg->SetLinePixel(atoi(m_sInput));
                    break;
            }
            m_iInputPos = 0;
            m_sInput[m_iInputPos] = '\0';
            gRefreshWorld = true;
            m_isDirty = true;
        }
            break;
        case KEY_F(10):
        {
            std::string newname = terrainCfg->GetName();
            if (newname.length()) terrainCfg->SaveData();

            std::string filename = world->GetTerrainFilename();
            gTerrainConfigMgr.RemoveMember(filename);
            delete(terrainCfg);

            if (filename != TerrainConfig::DEFAULT_NAME) filename = Path::GetBaseFilename(filename);
            TerrainConfig *editTerrain = gTerrainConfigMgr.GetMember(filename);
            if (editTerrain) {
                if(filename != newname) {
                    gTerrainConfigMgr.RemoveMember(filename);
                    delete(editTerrain);

                    std::string fullpath = Path::Combine(TerrainConfig::FILE_PATH,
                        (filename + TerrainConfig::FILE_EXTENSION).c_str());
                    Path::FileRemove(fullpath);
                }
            }
            if (newname.length()) {
                world->LoadTerrain(newname);
                UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
            } else {
                UIMgr::Instance()->CreateWnd<UIWndTerrainList>();
            }
            Destroy();
        }
            break;
        case KEY_ESC:
        {
            std::string filename = world->GetTerrainFilename();
            gTerrainConfigMgr.RemoveMember(filename);
            delete(terrainCfg);

            if (filename != TerrainConfig::DEFAULT_NAME) {
                filename = Path::GetBaseFilename(filename);
                world->LoadTerrain(filename);
                gRefreshWorld = true;
                UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
            } else {
                UIMgr::Instance()->CreateWnd<UIWndTerrainList>();
            }
            Destroy();
        }
            break;
        default:
        {
            if (ch > 0x1F && ch < 0x7F && m_iInputPos<maxInputLen) {
                // visible character
                m_sInput[m_iInputPos++] = ch;
                m_sInput[m_iInputPos] = '\0';
                m_isDirty = true;
            }
        }
            break;
    }
}
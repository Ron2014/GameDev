#include "UIWndTerrainList.h"

#include "UIMgr.h"
#include <vector>
#include <string>

#include "Global.h"
#include "Core/IO/Path.h"
#include "Core/Log/Log.h"
#include "Terrain/TerrainConfig.h"

#include "Entity/EntityPainter.h"
#include "World/WorldTerrainEditor.h"
#include "UIWndTerrainPainter.h"
#include "UIWndTerrainScene.h"
#include "UIWndTerrainPref.h"

UIWndTerrainList::UIWndTerrainList(/* args */):UIWnd(),
    m_filenames(nullptr),
    m_iCount(0),
    m_iChoice(0),
    m_iMenuLen(10)
{
    m_Anchor = UIWnd::ANCHOR::MIDDLE_CENTER;
    m_width = COLS - CURSES_BOADER * 2;
    m_height = LINES - CURSES_BOADER * 2; 
    m_keyWnd = true;
}

UIWndTerrainList::~UIWndTerrainList()
{
}

void UIWndTerrainList::OnResize() {
    SetTitle("Choose a terrain file to edit or create a new one");

	char* srcDir = TerrainConfig::FILE_PATH;
    int baselen = (int)strlen(srcDir);

    std::vector<std::string> fileList, dirList;
    if (!Path::DeepSearchDirectory(srcDir, fileList, dirList))
        Log::Error("ERROR: check dir path %s", srcDir);

    int len, i = 0;
    m_iCount = int(fileList.size()) + 1;
    m_filenames = (char **)malloc((m_iCount + 1) * sizeof(char *));

    for (auto &f: fileList) {
        i++;
        std::string filename = Path::GetBaseFilename(f);
        len = (int)(filename.length());
        if (len > m_iMenuLen) m_iMenuLen = len;
        m_filenames[i] = (char *)malloc((len + 1) * sizeof(char));
        strncpy(m_filenames[i], filename.c_str(), len);
        m_filenames[i][len] = '\0';
    }

    m_filenames[0] = (char *)malloc((m_iMenuLen + 1) * sizeof(char));
    strncpy(m_filenames[0], "<New File>", m_iMenuLen+1);
    
    m_filenames[m_iCount] = nullptr;
}

void UIWndTerrainList::OnUpdate() {
    int file_lines = m_height - 2; // 2 for title

    if (m_isDirty) {
        m_isDirty = false;
        
        char fmt[128];
        int i = 0, j;
        char **filename = m_filenames;
        while (*filename) {
            j = (i / file_lines) * (m_iMenuLen + 4);
            if (m_iChoice == i) {
                snprintf(fmt, 128, "* %%-%ds", m_iMenuLen + 2);
                wattron(m_pWnd, COLOR_PAIR(int(UIWnd::COLOR::MENU)));
                mvwprintw(m_pWnd, 3+i, 1+j, fmt, *filename);
                wattroff(m_pWnd, COLOR_PAIR(int(UIWnd::COLOR::MENU)));
            } else {
                snprintf(fmt, 128, "  %%-%ds", m_iMenuLen + 2);
                mvwprintw(m_pWnd, 3+i, 1+j, fmt, *filename);
            }
            i++;
            filename++;
        }
    }

    int ch;
    if ((ch = wgetch(m_pWnd)) != ERR) {
        switch (ch) {
            case KEY_ESC:
                gExitGame = true;
                break;
            case 'w':
            case KEY_UP:
            {
                m_iChoice = (m_iChoice + m_iCount - 1) % m_iCount;
                m_isDirty = true;
            }   
                break;
            case 's':
            case KEY_DOWN:
            {
                m_iChoice = (m_iChoice + 1) % m_iCount;
                m_isDirty = true;
            }   
                break;
            case 'a':
            case KEY_LEFT:
            {   
                if (m_iCount < file_lines) return;
                m_iChoice = (m_iChoice + m_iCount - file_lines) % m_iCount;
                m_isDirty = true;
            }
                break;
            case 'd':
            case KEY_RIGHT:
            {
                if (m_iCount < file_lines) return;
                m_iChoice = (m_iChoice + file_lines) % m_iCount;
                m_isDirty = true;
            }
                break;
            case '\n':
            {
                // loading or create terrain
                if (m_iChoice == 0) {
                    // mvwprintw(m_pWnd, m_height - 2, 1, "You have chosen %d item with name %s", m_iChoice,  m_filenames[m_iChoice]);

                    World *world = gWorldMgr.GetMember(gWatchingWorldID);
                    if (!world) {
                        world = new WorldTerrainEditor();
                        gWatchingWorldID = world->GetID();
                    }
                    
                    TerrainConfig *tmpTerrain = world->LoadTempTerrain();
                    UIMgr::Instance()->CreateWnd<UIWndTerrainPref>();
                    Destroy();

                } else {
                    char *filename = m_filenames[m_iChoice];
                    World *world = gWorldMgr.GetMember(gWatchingWorldID);
                    if (!world) {
                        world = new WorldTerrainEditor();
                        gWatchingWorldID = world->GetID();
                    }
                    world->LoadTerrain(filename);

                    UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
                    UIMgr::Instance()->CreateWnd<UIWndTerrainScene>();

                    Destroy();
                }
            }
                break;
        }
    }
}

void UIWndTerrainList::Clean() {
    for(int i=0; i<m_iCount; i++) {
        if (m_filenames[i]) free(m_filenames[i]);
    }
    free(m_filenames);

    wclear(m_pWnd);
    delwin(m_pWnd);

    m_filenames = nullptr;
    m_iCount = 0;
    m_iMenuLen = 10;
}
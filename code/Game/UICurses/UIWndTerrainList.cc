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

#if FUTURE_POSIX

UIWndTerrainList::UIWndTerrainList(/* args */):UIWnd(),
    m_filenames(nullptr),
    m_iCount(0),
    m_pMenu(nullptr),
    m_items(nullptr)
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

	char* srcDir = TerrainConfig::MAP_FILE_PATH;
    int baselen = (int)strlen(srcDir);

    std::vector<std::string> fileList, dirList;
    if (!Path::DeepSearchDirectory(srcDir, fileList, dirList))
        Log::Error("ERROR: check dir path %s", srcDir);

    int len, i = 0;
    m_iCount = fileList.size() + 1;
    m_items = (ITEM **)malloc((m_iCount + 1) * sizeof(ITEM *)); // the last item must be nullptr
    m_filenames = (char **)malloc(m_iCount * sizeof(char *));
    
    m_items[0] = new_item("New File", "");
    for (auto &f: fileList) {
        i++;
        std::string filename = Path::GetBaseFilename(f);
        len = (int)(filename.length());
        m_filenames[i] = (char *)malloc((len + 1) * sizeof(char));
        strncpy(m_filenames[i], filename.c_str(), len);
        m_filenames[i][len] = '\0';
        m_items[i] = new_item(m_filenames[i], "");
    }
    m_filenames[m_iCount-1] = nullptr;
    m_items[m_iCount] = nullptr;
    
	m_pMenu = new_menu(m_items);

    keypad(m_pWnd, TRUE);
    wtimeout(m_pWnd, CURSES_TIMEOUT);

    set_menu_win(m_pMenu, m_pWnd);
    set_menu_sub(m_pMenu, derwin(m_pWnd, m_height - 3, m_width, 3, 1)); // 3 lines for title
	set_menu_format(m_pMenu, m_height, 1);
    set_menu_mark(m_pMenu, "*");
        
	/* Post the menu */
	post_menu(m_pMenu);
}

void UIWndTerrainList::OnUpdate() {
    int c;
	if((c = wgetch(m_pWnd)) != ERR)
	{  
        switch(c) {
    	case KEY_DOWN:
            menu_driver(m_pMenu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(m_pMenu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(m_pMenu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(m_pMenu, REQ_SCR_UPAGE);
            break;
        case '\n':
        {
            // loading or create terrain
            ITEM *cur_item = current_item(m_pMenu);
            if (item_index(cur_item) == m_iCount - 1) {
                // create a new one
                wmove(m_pWnd, m_height - 2, 1);
                // wclrtoeol(m_pWnd);
                mvwprintw(m_pWnd, m_height - 2, 1, "You have chosen %d item with name %s and description %s", 
                item_index(cur_item) + 1,  item_name(cur_item), 
                item_description(cur_item));
                pos_menu_cursor(m_pMenu);
            } else {
                std::string filename = item_name(cur_item);
                World *world = gWorldMgr.GetMember(gWatchingWorldID);
                if (!world) {
                    world = new WorldTerrainEditor();
                    gWatchingWorldID = world->GetID();
                }
                world->LoadTerrain(filename);

                Entity *entity = gEntityMgr.GetMember(gContollingEntityID);
                if (!entity) {
                    entity = new EntityPainter();
                    gContollingEntityID = entity->GetID();
                }
                world->Enter(entity);

                // show pos & grid type
                UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
                // show terrain preference
                // UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
                // show scene
                UIMgr::Instance()->CreateWnd<UIWndTerrainScene>();

                Destroy();
            }
        }
            break;
        }
	}	
}

void UIWndTerrainList::Clean() {
    unpost_menu(m_pMenu);
    free_menu(m_pMenu);
    for(int i = 0; i < m_iCount; ++i) {
        if (m_filenames[i]) free(m_filenames[i]);
        free_item(m_items[i]);
    }
    free(m_filenames);

    wclear(m_pWnd);
    delwin(m_pWnd);

    m_filenames = nullptr;
    m_pMenu = nullptr;
    m_items = nullptr;
    m_iCount = 0;
}

#else


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

	char* srcDir = TerrainConfig::MAP_FILE_PATH;
    int baselen = (int)strlen(srcDir);

    std::vector<std::string> fileList, dirList;
    if (!Path::DeepSearchDirectory(srcDir, fileList, dirList))
        Log::Error("ERROR: check dir path %s", srcDir);

    int len, i = 0, j = 0;
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

    m_filenames[0] = (char *)malloc((len + 1) * sizeof(char));
    strncpy(m_filenames[0], "<New File>", m_iMenuLen+1);
    
    m_filenames[m_iCount] = nullptr;

    keypad(m_pWnd, TRUE);
    wtimeout(m_pWnd, CURSES_TIMEOUT);
}

void UIWndTerrainList::OnUpdate() {
    if (m_isDirty) {
        m_isDirty = false;
        
        char fmt[128];
        int i = 0, j;
        char **filename = m_filenames;
        while (*filename) {
            j = (i / LINES) * (m_iMenuLen + 4);
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
            case KEY_UP:
            {
                m_iChoice = (m_iChoice + m_iCount - 1) % m_iCount;
                m_isDirty = true;
            }   
                break;
            case KEY_DOWN:
            {
                m_iChoice = (m_iChoice + m_iCount + 1) % m_iCount;
                m_isDirty = true;
            }   
                break;
            case '\n':
            {
                // loading or create terrain
                if (m_iChoice == m_iCount - 1) {
                    mvwprintw(m_pWnd, m_height - 2, 1, "You have chosen %d item with name %s", m_iChoice,  m_filenames[m_iChoice]);

                } else {
                    char *filename = m_filenames[m_iChoice];
                    World *world = gWorldMgr.GetMember(gWatchingWorldID);
                    if (!world) {
                        world = new WorldTerrainEditor();
                        gWatchingWorldID = world->GetID();
                    }
                    world->LoadTerrain(filename);

                    Entity *entity = gEntityMgr.GetMember(gContollingEntityID);
                    if (!entity) {
                        entity = new EntityPainter();
                        gContollingEntityID = entity->GetID();
                    }
                    world->Enter(entity);

                    // show pos & grid type
                    UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
                    // show terrain preference
                    // UIMgr::Instance()->CreateWnd<UIWndTerrainPainter>();
                    // show scene
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

#endif
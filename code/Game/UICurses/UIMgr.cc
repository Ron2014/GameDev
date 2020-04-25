#include "UIMgr.h"
#include "UIWnd.h"
#include <set>

#if FUTURE_WINDOWS
#include <windows.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

int UIMgr::m_iNextValidID = 0;

UIMgr::UIMgr(/* args */)
{
	initscr();
    curs_set(FALSE);
	cbreak();
    noecho();
    
    // color
	if(has_colors() == FALSE) {
        endwin();
		printf("Your terminal does not support color\n");
        exit(1);
    }
	start_color();			/* Start color 			*/
	init_pair((int)UIWnd::COLOR::MENU, COLOR_BLACK, COLOR_WHITE);
    
    refresh();
}

UIMgr::~UIMgr()
{
    endwin();
}

void UIMgr::Update() {
    std::set<int> destroyList;

    UIWnd *pWnd;
    std::map<int, UIWnd*>::iterator it = m_windows.begin();
    
    while(it != m_windows.end()) {
        pWnd = it->second;
        if (pWnd->NeedDestroy())
            destroyList.insert(pWnd->GetID());
        else
            pWnd->Update();
        ++it;
    }

    int wndID;
    for(std::set<int>::iterator it0 = destroyList.begin(); it0 != destroyList.end(); ++it0) {
        wndID = *it0;
        pWnd = m_windows.at(wndID);
        m_windows.erase(wndID);
        pWnd->Clean();
        pWnd->OnDestroy();
        free(pWnd);
    }
}
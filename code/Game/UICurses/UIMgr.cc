#include "UIMgr.h"
#include "UIWnd.h"

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
    refresh();
}

UIMgr::~UIMgr()
{
    endwin();
}

void UIMgr::DestroyWnd(UIWnd *pWnd) {
    m_windows.erase(pWnd->GetID());
    free(pWnd);
}

void UIMgr::Update() {
    for (std::map<int, UIWnd*>::iterator it = m_windows.begin(); it != m_windows.end(); it++)
        it->second->Update();
}
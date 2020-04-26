#include "UIMgr.h"
#include "UIWnd.h"
#include <cstring>

const UIWnd::TYPE UIWnd::type = UIWnd::none;

UIWnd::UIWnd(/* args */):
    m_ID(-1),
    m_pWnd(nullptr),
    m_height(0),
    m_width(0),
    m_Anchor(UIWnd::ANCHOR::LEFT_TOP),
    m_offsetX(0),
    m_offsetY(0),
    m_destroy(0),
    m_isDirty(true),
    m_keyWnd(false)
{
}

UIWnd::~UIWnd()
{
}

void UIWnd::Clean() {
    wclear(m_pWnd);
    delwin(m_pWnd);
}

void UIWnd::Resize() {
    if(m_pWnd) Clean();
    
    int height = m_height + CURSES_BOADER * 2;
    int width = m_width + CURSES_BOADER * 2;

    int starty, startx;
    switch(m_Anchor) {
        case LEFT_TOP:
            starty = 0; 
            startx = 0;
            break;
        case MIDDLE_TOP:
            starty = 0;
            startx = (COLS - width) / 2;
            break;
        case RIGHT_TOP:
            starty = 0;
            startx = COLS - width;
            break;
        case LEFT_CENTER:
            starty = (LINES - height) / 2;
            startx = 0;
            break;
        case MIDDLE_CENTER:
            starty = (LINES - height) / 2;
            startx = (COLS - width) / 2;
            break;
        case RIGHT_CENTER:
            starty = (LINES - height) / 2;
            startx = COLS - width;
            break;
        case LEFT_BOTTOM:
            starty = LINES - height;
            startx = 0;
            break;
        case MIDDLE_BOTTOM:
            starty = LINES - height;
            startx = (COLS - width) /2;
            break;
        case RIGHT_BOTTOM:
            starty = LINES - height;
            startx = COLS - width;
            break;
    }
    starty += m_offsetY;
    startx += m_offsetX;
    m_pWnd = newwin(height, width, starty, startx);
    box(m_pWnd, 0 , 0);

    OnResize();
}

void UIWnd::Init() {
    Resize();
    OnInit();
}

void UIWnd::Update() {
    if (m_destroy || !NeedUpdate()) return;
    OnUpdate();
    if (!m_destroy) box(m_pWnd, 0, 0);
    wrefresh(m_pWnd);
}

void UIWnd::Destroy() {
    m_destroy++;
    wclear(m_pWnd);
}

void UIWnd::SetTitle(char *msg, int row) {
	mvwaddch(m_pWnd, row-1, 0, ACS_LTEE);
	mvwhline(m_pWnd, row-1, 1, ACS_HLINE, m_width);
	mvwaddch(m_pWnd, row-1, m_width + CURSES_BOADER * 2 - 1, ACS_RTEE);

    printw_in_middle(row, msg);
    
	mvwaddch(m_pWnd, row+1, 0, ACS_LTEE);
	mvwhline(m_pWnd, row+1, 1, ACS_HLINE, m_width);
	mvwaddch(m_pWnd, row+1, m_width + CURSES_BOADER * 2 - 1, ACS_RTEE);
}

void UIWnd::printw_in_middle(int starty, char *msg){
    int len = (int)strlen(msg);
    int startx = (m_width - len) / 2;
    mvwprintw(m_pWnd, starty, startx, msg);
}
#pragma once

#include <curses.h>
#include <menu.h>

class UIWnd
{
public:
    enum TYPE {
        none,
        scene,
        control,
        terrain_list,
        terrain_pref,
        terrain_painter,
        terrain_scene,
    };

    enum ANCHOR {
        LEFT_TOP,    MIDDLE_TOP,    RIGHT_TOP,
        LEFT_CENTER, MIDDLE_CENTER, RIGHT_CENTER,
        LEFT_BOTTOM, MIDDLE_BOTTOM, RIGHT_BOTTOM,
    };
    
protected:
    /* data */
    int m_ID;

    WINDOW * m_pWnd;
    int m_width;
    int m_height;

    /**
     * 0: valid
     * >1: Destroy called (any times)
     * -1: Already Destroied (only once)
    */
    int m_destroy;  

    ANCHOR m_Anchor;
    int m_offsetX;
    int m_offsetY;

    virtual bool NeedUpdate() { return true; }
    virtual void OnInit() {}        // draw content once
    virtual void OnResize() {}      // draw content everytime
    virtual void OnUpdate() {}      // logic before refresh
    virtual void OnDestroy() {}     // logic after clean

    UIWnd(/* args */);

    void SetTitle(char *msg);
    void printw_in_middle(int starty, char *msg);

public:
    static const UIWnd::TYPE type;
    virtual ~UIWnd();

    void SetID(int id) { m_ID = id; }
    int GetID() { return m_ID; }

    WINDOW *GetWnd() { return m_pWnd; }

    void Resize();
    virtual void Clean();
    void Init();
    void Update();
    void Destroy();
};

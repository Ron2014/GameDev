#pragma once
#include "Core/Object/ObjectManager.h"
#include "Core/Singleton.h"
#include "UIWnd.h"
#include <map>
using namespace Future;

#define CURSES_BOADER 1
#define CURSES_TIMEOUT 30


class UIMgr
{
private:
    std::map<int, UIWnd*> m_windows;
    std::map<UIWnd::TYPE, int> m_singleons;

    static int m_iNextValidID;

public:
    UIMgr(/* args */);
    ~UIMgr();

    template <class T>
    T *CreateWnd() {
        T* pWnd = new T();
        pWnd->SetID(m_iNextValidID);
        pWnd->Init();

        m_iNextValidID++;
        if (m_iNextValidID < 0)
            m_iNextValidID = 0;
        
        m_windows.insert(std::pair<int, UIWnd*>(pWnd->GetID(), pWnd));
        if (T::type) m_singleons.insert(std::pair<UIWnd::TYPE, int>(T::type, pWnd->GetID()));

        return pWnd;
    }

    template <class T>
    T *GetWnd() {
        std::map<UIWnd::TYPE, int>::iterator it = m_singleons.find(T::type);
        if (it != m_singleons.end()) {
            std::map<int, UIWnd*>::iterator it0 = m_windows.find(it->second);
            if (it0 != m_windows.end()) return (T *)it0->second;
        }
        return nullptr;
    }

    template <class T>
    void UpdateWnd() {
        T *pWnd = GetWnd<T>();
        if (pWnd) pWnd->Update();
    }

    void DestroyWnd(UIWnd *);
    void Update();

    SINGLETON(UIMgr)
};
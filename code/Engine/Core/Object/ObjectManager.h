#pragma once
#include <cassert>
#include <map>

namespace Future {

template <class T>
class ObjectManager
{
private:
    std::map<int, T*> m_map;

public:
    ObjectManager();
    virtual ~ObjectManager();

    virtual void Update();

    virtual void AddMember(int, T*);
    virtual void RemoveMember(const int);
    virtual T* GetMember(const int);
};

template <class T>
void ObjectManager<T>::AddMember(int id, T* e){
    assert((m_map.find(id) == m_map.end()) && "member already exist!");
    m_map.insert(std::make_pair(id, e));
}

template <class T>
void ObjectManager<T>::RemoveMember(const int id){
    assert((m_map.find(id) != m_map.end()) && "member don't exist!");
    m_map.erase(id);
}

template <class T>
T* ObjectManager<T>::GetMember(const int id){
    std::map<int, T*>::iterator it = m_map.find(id);
    if (it != m_map.end())
        return it->second;
    return nullptr;
}

} // namespace Future
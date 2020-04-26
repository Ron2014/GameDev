#pragma once
#include <cassert>
#include <map>

namespace Future {

template <typename TK, class TV>
class ObjectManager
{
private:
    std::map<TK, TV*> m_map;

public:
    ObjectManager();
    virtual ~ObjectManager();

    virtual void AddMember(const TK &, TV*);
    virtual void RemoveMember(const TK &);
    virtual TV* GetMember(const TK &);
};

template <typename TK, class TV>
ObjectManager<TK, TV>::ObjectManager() {

}

template <typename TK, class TV>
ObjectManager<TK, TV>::~ObjectManager() {

}

template <typename TK, class TV>
void ObjectManager<TK, TV>::AddMember(const TK &id, TV* e){
    assert((m_map.find(id) == m_map.end()) && "member already exist!");
    m_map.insert(std::make_pair(id, e));
}

template <typename TK, class TV>
void ObjectManager<TK, TV>::RemoveMember(const TK &id){
    assert((m_map.find(id) != m_map.end()) && "member don't exist!");
    m_map.erase(id);
}

template <typename TK, class TV>
TV* ObjectManager<TK, TV>::GetMember(const TK &id){
    typename std::map<TK, TV*>::iterator it = m_map.find(id);
    if (it != m_map.end())
        return it->second;
    return nullptr;
}

} // namespace Future
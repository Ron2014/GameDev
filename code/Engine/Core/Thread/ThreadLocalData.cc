//------------------------------------------------------------------------------
//  ThreadLocalData.cc
//------------------------------------------------------------------------------
#include "StdFuture.h"
#include "ThreadLocalData.h"
#include "Core/Memory/Memory.h"
#include "Core/Assertion.h"

#if FUTURE_THREADLOCAL_PTHREAD
namespace Future {

pthread_key_t ThreadLocalData::key = 0;
std::atomic<int> ThreadLocalData::curSlot{ 0 };

//------------------------------------------------------------------------------
void
ThreadLocalData::SetupOnce() {
    if (0 == key) {
        pthread_key_create(&key, 0);
        f_assert_dbg(0 != key);
    }
}

//------------------------------------------------------------------------------
void**
ThreadLocalData::PointerTable() {
    f_assert_dbg(0 != key);

    // get thread-local value (null if none assigned yet)
    void** table = (void**)pthread_getspecific(key);
    if (0 == table) {
        // not assigned yet, allocate thread-specific table and
        // associate with key
        int tableSize = MaxNumSlots * sizeof(void*);
        table = (void**)Memory::Alloc(tableSize);
        Memory::Clear(table, tableSize);
        pthread_setspecific(key, table);
    }
    return table;
}

//------------------------------------------------------------------------------
int
ThreadLocalData::Alloc() {
    f_assert_dbg(curSlot < (MaxNumSlots - 1));
    return curSlot++;
}

//------------------------------------------------------------------------------
void
ThreadLocalData::Set(int slotIndex, void* ptr) {
    f_assert_range_dbg(slotIndex, MaxNumSlots);
    void** table = PointerTable();
    table[slotIndex] = ptr;
}

} // namespace Future
#else
// silence 'has no symbols' warning
FUTURE_UNUSED int future_thread_local_dummy = 0;
#endif

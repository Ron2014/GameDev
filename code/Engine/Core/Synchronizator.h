#pragma once
#include <thread>
#include <mutex>

#include "RefCounted.h"
#include "Ptr.h"

#include "Core/Util/ObjectAllocator.h"
#include "Core/Util/MemoryBlock.h"


namespace Future {
class Synchronizator {
public:
	Synchronizator();
	virtual ~Synchronizator();

public:
	typedef void(RefCounted::*SyncProc) (void* param);

public:
	bool InOwnThread();
	std::thread::id GetOwnThreadId();

	template<typename TargetType>
	void SyncCall(const Ptr<TargetType>& target, void(TargetType::*callback)(void* param), void *param) {
		static_assert(std::is_base_of<RefCounted, TargetType>::value, "Target type must base from RefCounted");
		this->DoSyncCall(target, (SyncProc)callback, param);
	}

	template<typename TargetType>
	void AsyncCall(const Ptr<TargetType>& target, void(TargetType::*callback)(void* param), void *param) {
		static_assert(std::is_base_of<RefCounted, TargetType>::value, "Target type must base from RefCounted");
		this->DoAsyncCall(target, (SyncProc)callback, param);
	}

	void ProcessCall();

protected:
	void DoSyncCall(const Ptr<RefCounted>& target, SyncProc func, void *param);
	void DoAsyncCall(const Ptr<RefCounted>& target, SyncProc func, void *param);

	
protected:
	struct ExecFlag {
		enum Code {
			Wait,
			Excuting,
			Finish,
			Complete
		};
	};

	struct Task {
		RefCounted *target;
		SyncProc func;
		void* param;
		std::atomic<uint32_t> flag;
		bool sync;
	};

private:
	typedef MemoryBlock<Task*> TaskList;

private:
	std::thread::id m_OwnerThreadId;

	std::mutex  m_AllocatorLocker;
	ObjectAllocator<Task> m_TaskAllocator;

	TaskList m_TaskList[2];
	TaskList *m_pTaskProcessList;
	TaskList *m_pTaskAppendList;
	uint32_t  m_CurrentProcessIndex;
	std::mutex m_TaskListLocker;

};
}
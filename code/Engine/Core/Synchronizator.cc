#include "Synchronizator.h"
#include <atomic>

namespace Future {

Synchronizator::Synchronizator() {
	this->m_OwnerThreadId = std::this_thread::get_id();
	this->m_pTaskProcessList = this->m_TaskList;
	this->m_pTaskAppendList = this->m_TaskList + 1;
	this->m_CurrentProcessIndex = 0;
}

Synchronizator::~Synchronizator() {
	std::lock_guard<std::mutex> lock{ this->m_TaskListLocker };
	for (int32_t i = 0; i < 2; ++i) {
		this->ProcessCall();
	}
}

bool 
Synchronizator::InOwnThread() {
	return std::this_thread::get_id() == this->m_OwnerThreadId;
}

std::thread::id 
Synchronizator::GetOwnThreadId() {
	return this->m_OwnerThreadId;
}

void 
Synchronizator::DoSyncCall(const Ptr<RefCounted>& target, SyncProc func, void *param) {
	if (std::this_thread::get_id() == this->GetOwnThreadId()) {
		auto ptr = target.get();
		(ptr->*func)(param);
	}
	else {
		this->m_AllocatorLocker.lock();
		auto task = this->m_TaskAllocator.allocObject();
		this->m_AllocatorLocker.unlock();

		task->target = target.get();
		task->func = func;
		task->param = param;
		task->flag = ExecFlag::Wait;
		task->sync = true;

		{
			std::lock_guard<std::mutex> lock{ this->m_TaskListLocker };
			this->m_pTaskAppendList->append(task);
		}

		uint32_t exp = ExecFlag::Finish;

		while (task->flag.compare_exchange_strong(exp, (uint32_t)ExecFlag::Complete)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		this->m_AllocatorLocker.lock();
		m_TaskAllocator.freeObject(task);
		this->m_AllocatorLocker.unlock();
	}
}

void 
Synchronizator::DoAsyncCall(const Ptr<RefCounted>& target, SyncProc func, void *param) {
	this->m_AllocatorLocker.lock();
	auto task = this->m_TaskAllocator.allocObject();
	this->m_AllocatorLocker.unlock();

	task->target = target.get();
	task->func = func;
	task->param = param;
	task->flag = ExecFlag::Wait;
	task->sync = false;

    // for avoid object invalid in async progress.
	target->addRef();

	{
		std::lock_guard<std::mutex> lock{ this->m_TaskListLocker };
		this->m_pTaskAppendList->append(task);
	}
}

void 
Synchronizator::ProcessCall() {
	if (this->m_CurrentProcessIndex >= (uint32_t)this->m_pTaskProcessList->size()) {
		if (this->m_pTaskAppendList->size() > 0) {
			std::lock_guard<std::mutex> lock{ this->m_TaskListLocker };
			this->m_pTaskProcessList->clear(false);
			auto tmp = this->m_pTaskProcessList;
			this->m_pTaskProcessList = this->m_pTaskAppendList;
			this->m_pTaskAppendList = tmp;
			this->m_CurrentProcessIndex = 0;
		}
	}

	while (this->m_CurrentProcessIndex < (uint32_t)m_pTaskProcessList->size()) {
		auto task = *this->m_pTaskProcessList->get(this->m_CurrentProcessIndex);
		this->m_CurrentProcessIndex++;

		const bool sync = task->sync;

		uint32_t exp = ExecFlag::Wait;
		if (!sync || task->flag.compare_exchange_strong(exp, (uint32_t)ExecFlag::Excuting)) {
			auto target = task->target;
			(target->*task->func)(task->param);

			if (sync) {
                // have referenced in sync mode. don't need dereference here
				task->flag.store((uint32_t)ExecFlag::Finish);
			}
			else {
				target->release();

				this->m_TaskListLocker.lock();
				this->m_TaskAllocator.freeObject(task);
				this->m_TaskListLocker.unlock();
			}
		}
	}
}

}
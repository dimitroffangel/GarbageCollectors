#include "BuddyAllocatorObject.h"

#include "BuddyAllocator.h"

std::mutex BuddyAllocatorObject::m_LockOperation;

void* BuddyAllocatorObject::operator new(size_t size)
{
	//std::lock_guard<std::mutex> guardian(m_LockOperation);

	return g_BuddyAllocator->Allocate(size);
}

void BuddyAllocatorObject::operator delete(void* pointer, size_t size)
{
	//std::lock_guard<std::mutex> guardian(m_LockOperation);

	g_BuddyAllocator->Deallocate(pointer, size);
}

void BuddyAllocatorObject::operator delete(void* pointer)
{
	//std::lock_guard<std::mutex> guardian(m_LockOperation);

	g_BuddyAllocator->Free(pointer);
}


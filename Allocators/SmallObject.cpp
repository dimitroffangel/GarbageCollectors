#include "SmallObject.h"

std::mutex SmallObject::m_LockOperation;

void* SmallObject::operator new(size_t size)
{
	//std::lock_guard<std::mutex> guardian(m_LockOperation);

	return g_SmallObjectAllocator->Allocate(size);
}

void SmallObject::operator delete(void* pointer, size_t size)
{
	//std::lock_guard<std::mutex> guardian(m_LockOperation);

	g_SmallObjectAllocator->Deallocate(pointer, size);
}

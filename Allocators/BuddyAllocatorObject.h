#ifndef BUDDYALLOCATOROBJECT_H_GUARD
#define BUDDYALLOCATOROBJECT_H_GUARD

#include <mutex>

class BuddyAllocatorObject
{
public:
	__declspec(allocator) static void* operator new(size_t size);
	__declspec(allocator) static void operator delete(void* pointer, size_t size);
	__declspec(allocator) static void operator delete(void* pointer);


private:
	static std::mutex m_LockOperation;
};

#endif


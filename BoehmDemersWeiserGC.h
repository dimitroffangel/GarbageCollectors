#pragma once

#include "Object.h"
#include "Allocators/SmallObjectAllocator.h"

class BoehmDemersWeiserGC : public GarbageCollector
{
public:
	Object** m_Root = nullptr;
	std::unordered_set<Object*> m_Allocated;
	std::unordered_set<Object*> m_Visited;
	size_t m_GCInterval;
	size_t m_AllocsSinceGC = 0;

public:
	BoehmDemersWeiserGC(const size_t gcInterval);
	
	// Inherited via GarbageCollector
	virtual void VisitReference(Object* from, Object** to, void* state) override
	{

	}

	virtual void* Allocate(size_t size) override;

	virtual void SetRoot(Object** root) override
	{
		m_Root = root;
	}

	virtual void Shutdown() override;

	void CollectGarbage();

	//void operator delete(void* pointer, size_t size)
	//{
	//	g_SmallObjectAllocator->Deallocate(pointer, size);
	//}

	//void* operator new(size_t size)
	//{
	//	return g_SmallObjectAllocator->Allocate(size);
	//}

};


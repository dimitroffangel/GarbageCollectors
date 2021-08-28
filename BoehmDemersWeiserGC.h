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
	BoehmDemersWeiserGC(const size_t gcInterval)
		:m_GCInterval(gcInterval)
	{
		
	}

	// Inherited via GarbageCollector
	virtual void VisitReference(Object* from, Object** to, void* state) override
	{

	}

	virtual void* Allocate(size_t size) override
	{
		Object* const  allocatedObject = static_cast<Object*>(g_SmallObjectAllocator->Allocate(size));
		m_Allocated.insert(allocatedObject);

		return allocatedObject;
	}
	virtual void SetRoot(Object** root) override
	{
		m_Root = root;
	}

	virtual void Shutdown() override;

	void CollectGarbage()
	{
		m_Visited.clear();

		// TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
		if (m_Root == nullptr)
		{
			return;
		}

		m_Visited.insert(*m_Root);
		(*m_Root)->VisitReferences(this, nullptr);

		for (const auto& object : m_Allocated)
		{
			if (m_Visited.find(object) == m_Visited.end())
			{
				object->~Object();
				g_SmallObjectAllocator->Deallocate(object, sizeof(object));
			}
		}

		// compact the region


		m_Allocated = m_Visited;
	}
};


#pragma once

#include "Object.h"
#include "Allocators/LinearAllocator.h"

class ChineyAlgorithmGC : public GarbageCollector
{
public:
	Object** m_Root = nullptr;
	std::unordered_set<Object*> m_Allocated;
	std::unordered_set<Object*> m_Visited;
	size_t m_GCInterval;
	size_t m_AllocsSinceGC = 0;
    LinearAllocator m_FromSpace;
    LinearAllocator m_ToSpace;

	// Inherited via GarbageCollector
	virtual void VisitReference(Object* from, Object** to, void* state) override
	{

	}


	virtual void* Allocate(const size_t size) override
	{
		Object* const allocatedMemory = static_cast<Object*>(m_FromSpace.Allocate(size));

		m_Allocated.insert(allocatedMemory);

		return allocatedMemory;
	}


    void CollectGarbage()
    {
		FlipSpaces();
    }

    void Shutdown() override
    {
    }

	virtual void SetRoot(Object** root) override
	{
		m_Root = root;
	}

private:
	void FlipSpaces()
	{
		std::swap(m_FromSpace, m_ToSpace);
		
		Copy(*m_Root);
	}

	void Copy(Object*& objectToCopy)
	{
		if (objectToCopy->m_ForwardPointer != nullptr)
		{
			return;
		}

		objectToCopy->m_ForwardPointer = static_cast<Object*>(m_ToSpace.Allocate(objectToCopy->m_Size));
		std::memcpy(objectToCopy->m_ForwardPointer, objectToCopy, objectToCopy->m_Size);

		for (size_t i = 0; i < objectToCopy->m_NumberOfReferredObjects; ++i)
		{
			Copy(objectToCopy->m_ReferedObjects[i]);
			objectToCopy->m_ReferedObjects[i] = objectToCopy->m_ForwardPointer;
		}
	}
};


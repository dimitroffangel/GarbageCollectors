#pragma once

#include "Object.h"

class ChineyAlgorithmGC : public GarbageCollector
{
public:
	Object** m_Root = nullptr;
	std::unordered_set<Object*> m_Allocated;
	std::unordered_set<Object*> m_Visited;
	size_t m_GCInterval;
	size_t m_AllocsSinceGC = 0;
    void* m_FromSpace;
    void* m_ToSpace;

	// Inherited via GarbageCollector
	virtual void VisitReference(Object* from, Object** to, void* state) override;


	virtual void* Allocate(size_t size) override
	{

	}


    void CollectGarbage()
    {

    }

    void Shutdown() override
    {
    }

	virtual void SetRoot(Object** root) override;
};


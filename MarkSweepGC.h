#pragma once

#include "Object.h"

class MarkSweepGC : public GarbageCollector
{
public:
    Object** m_Root = nullptr;
    std::unordered_set<Object*> m_Allocated;
    std::unordered_set<Object*> m_Visited;
    size_t m_GCInterval;
    size_t m_AllocsSinceGC = 0;

public:
	MarkSweepGC(const size_t gcInterval)
		: m_GCInterval(gcInterval)
	{

	}

    void* Allocate(const size_t size) override
    {
        if (++m_AllocsSinceGC == m_GCInterval)
        {
            CollectGarbage();

        }
    }

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
        
        for (const auto object : m_Allocated)
        {
            if (m_Visited.find(object) == m_Visited.end())
            {
                object->~Object();
                ::operator delete(object);
            }
        }

        m_Allocated = m_Visited;
    }

    void Shutdown() override
    {
        for (const auto object : m_Allocated)
        {
            object->~Object();
            ::operator delete(object);
        }
    }
};


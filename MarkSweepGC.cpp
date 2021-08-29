#include "MarkSweepGC.h"

MarkSweepGC::MarkSweepGC(const size_t gcInterval)
	: m_GCInterval(gcInterval)
{

}

void* MarkSweepGC::Allocate(const size_t size)
{
    if (++m_AllocsSinceGC == m_GCInterval)
    {
        CollectGarbage();
        m_AllocsSinceGC = 0;
    }

    auto memory = ::operator new(size);
    m_Allocated.insert(static_cast<Object*>(memory));
    return memory;
}

void MarkSweepGC::CollectGarbage()
{
    m_Visited.clear();

    // TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
    if (m_Root == nullptr)
    {
        return;
    }

    (*m_Root)->VisitReferences(this, nullptr, m_Visited);

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

void MarkSweepGC::Shutdown()
{
    m_Visited.clear();

    // TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
    if (m_Root == nullptr)
    {
        return;
    }

    m_Visited.insert(*m_Root);
    (*m_Root)->VisitReferences(this, nullptr, m_Visited);

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
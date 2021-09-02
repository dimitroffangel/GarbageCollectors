#include "MarkSweepGC.h"

//#include "Object.h"
#include <memory>

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
    m_Visited.insert(*m_Root);
    (*m_Root)->VisitReferences(this, nullptr);

    for (auto object : m_Allocated)
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
    for (auto object : m_Allocated)
    {
        object->~Object();
        ::operator delete(object);
    }
}

void MarkSweepGC::VisitReference(Object* from, Object** to, void* state)
{
    auto insert = m_Visited.insert(*to);
    if (insert.second)
    {
        (*to)->VisitReferences(this, nullptr);
    }
}

std::unique_ptr<GarbageCollector> CreateGarbageCollector(int argc, char* argv[])
{
    return std::unique_ptr<GarbageCollector>(new MarkSweepGC(1024));
}
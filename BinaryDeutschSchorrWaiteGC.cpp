#include "BinaryDeutschSchorrWaiteGC.h"

void BinaryDeutschSchorrWaiteGC::VisitReference(Object* from, Object** to, void* state)
{
	CollectGarbage();
}

void BinaryDeutschSchorrWaiteGC::Shutdown()
{

    m_Visited.clear();

    // TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
    if (m_Root == nullptr)
    {
        return;
    }

    for (const auto object : m_Allocated)
    {
        if (m_Visited.find(object) == m_Visited.end())
        {
            object->~BDSW_Binary_Object();
            ::operator delete(object);
        }
    }

    m_Allocated = m_Visited;
}

#include "BinaryDeutschSchorrWaiteGC.h"

BinaryDeutschSchorrWaiteGC::BinaryDeutschSchorrWaiteGC(const size_t gcInterval)
    :m_GCInterval(gcInterval)
{

}
void* BinaryDeutschSchorrWaiteGC::Allocate(const size_t size)
{
    if (++m_AllocsSinceGC == m_GCInterval)
    {
        CollectGarbage();
        m_AllocsSinceGC = 0;
    }

    auto memory = ::operator new(size);
    m_Allocated.insert(static_cast<BDSW_Binary_Object*>(memory));
    return memory;
}

void BinaryDeutschSchorrWaiteGC::CollectGarbage()
{
    m_Visited.clear();

    if (m_Root != nullptr)
    {
        return;
    }

    bool hasFinished = false;
    BDSW_Binary_Object* currentNode = *m_Root;
    BDSW_Binary_Object* previousNode = nullptr;
    BDSW_Binary_Object* nextNode = nullptr;

    while (!hasFinished)
    {
        // follow left pointer
        while (currentNode != nullptr && m_Visited.find(currentNode) == m_Visited.end())
        {
            m_Visited.insert(currentNode);
            if (currentNode->m_ReferedObjects != nullptr)
            {
                nextNode = currentNode->m_ReferedObjects[0];
                currentNode->m_ReferedObjects[0] = previousNode;
                previousNode = currentNode;
                currentNode = nextNode;
            }
        }

        // retreat to the root
        while (previousNode != nullptr && previousNode->m_HasIteratedLeftChild)
        {
            previousNode->m_HasIteratedLeftChild = false;
            nextNode = previousNode->m_ReferedObjects[1];
            previousNode->m_ReferedObjects[1] = currentNode;
            currentNode = previousNode;
            previousNode = nextNode;
        }

        if (previousNode == nullptr)
        {
            hasFinished = true;
        }

        else
        {
            previousNode->m_HasIteratedLeftChild = true;
            nextNode = previousNode->m_ReferedObjects[0];
            previousNode->m_ReferedObjects[0] = currentNode;
            currentNode = previousNode->m_ReferedObjects[1];
            previousNode->m_ReferedObjects[1] = nextNode;
        }
    }
}

void BinaryDeutschSchorrWaiteGC::VisitReference(Object* from, Object** to, void* state)
{

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

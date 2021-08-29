#include "DeutschSchorrWaite_VariableSizeNode_GC.h"

DeutschSchorrWaite_VariableSizeNode_GC::DeutschSchorrWaite_VariableSizeNode_GC(const size_t gcInterval)
    :m_GCInterval(gcInterval)
{

}

void* DeutschSchorrWaite_VariableSizeNode_GC::Allocate(const size_t size)
{
    if (++m_AllocsSinceGC == m_GCInterval)
    {
        m_AllocsSinceGC = 0;
        CollectGarbage();
    }

    auto memory = ::operator new(size);
    m_Allocated.insert(static_cast<DeutschSchorrWaite_VariableSizeNode*>(memory));
    return memory;
}

void DeutschSchorrWaite_VariableSizeNode_GC::CollectGarbage()
{
    m_Visited.clear();

    if (m_Root != nullptr)
    {
        return;
    }

    bool hasFinished = false;
    DeutschSchorrWaite_VariableSizeNode* currentNode = *m_Root;
    DeutschSchorrWaite_VariableSizeNode* previousNode = nullptr;
    DeutschSchorrWaite_VariableSizeNode* nextNode = nullptr;

    while (!hasFinished)
    {
        // iterate children
        for (size_t i = currentNode->m_CurrentIteratedChild; i < currentNode->m_NumberOfChildren; ++i)
        {
            if (currentNode->m_ReferedObjects[i] != nullptr &&
                m_Visited.find(currentNode->m_ReferedObjects[i]) == m_Visited.end())
            {
                m_Visited.insert(currentNode->m_ReferedObjects[i]);
                ++currentNode->m_CurrentIteratedChild;
                if (currentNode->m_ReferedObjects[i]->m_ReferedObjects != nullptr)
                {
                    nextNode = currentNode->m_ReferedObjects[i];
                    currentNode->m_ReferedObjects[i] = previousNode;
                    previousNode = currentNode;
                    currentNode = nextNode;
                    i = currentNode->m_CurrentIteratedChild;
                }
            }
        }

        // now go back
        // retreat to the root
        while (previousNode != nullptr &&
            previousNode->m_NumberOfChildren == previousNode->m_CurrentIteratedChild)
        {
            previousNode->m_CurrentIteratedChild = 0;
            nextNode = previousNode->m_ReferedObjects[previousNode->m_CurrentIteratedChild - 1];
            previousNode->m_ReferedObjects[previousNode->m_CurrentIteratedChild - 1] = currentNode;
            currentNode = previousNode;
            previousNode = nextNode;
        }

        if (previousNode == nullptr)
        {
            hasFinished = true;
        }
    }
}

void DeutschSchorrWaite_VariableSizeNode_GC::VisitReference(Object* from, Object** to, void* state)
{

}

void DeutschSchorrWaite_VariableSizeNode_GC::SetRoot(Object** root)
{

}

void DeutschSchorrWaite_VariableSizeNode_GC::Shutdown()
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
            object->~DeutschSchorrWaite_VariableSizeNode();
            ::operator delete(object);
        }
    }

    m_Allocated = m_Visited;
}

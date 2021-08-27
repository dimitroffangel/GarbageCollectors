#pragma once
#include "Object.h"

class DeutschSchorrWaite_VariableSizeNode_GC : public GarbageCollector
{
public:
    DeutschSchorrWaite_VariableSizeNode** m_Root = nullptr;
    std::unordered_set<DeutschSchorrWaite_VariableSizeNode*> m_Allocated;
    std::unordered_set<DeutschSchorrWaite_VariableSizeNode*> m_Visited;
    size_t m_GCInterval;
    size_t m_AllocsSinceGC = 0;


public:
    DeutschSchorrWaite_VariableSizeNode_GC(const size_t gcInterval)
        :m_GCInterval(gcInterval)
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
                nextNode = previousNode->m_ReferedObjects[previousNode->m_CurrentIteratedChild-1];
                previousNode->m_ReferedObjects[previousNode->m_CurrentIteratedChild-1] = currentNode;
                currentNode = previousNode;
                previousNode = nextNode;
            }

            if (previousNode == nullptr)
            {
                hasFinished = true;
            }
        }
    }
};


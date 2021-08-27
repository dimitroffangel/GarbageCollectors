#pragma once
#include "Object.h"

class BinaryDeutschSchorrWaiteGC : public GarbageCollector
{
public:
    BDSW_Binary_Object** m_Root = nullptr;
    std::unordered_set<BDSW_Binary_Object*> m_Allocated;
    std::unordered_set<BDSW_Binary_Object*> m_Visited;
    size_t m_GCInterval;
    size_t m_AllocsSinceGC = 0;

    
public:
    BinaryDeutschSchorrWaiteGC(const size_t gcInterval)
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
};


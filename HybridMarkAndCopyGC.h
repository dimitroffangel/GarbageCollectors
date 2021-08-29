#pragma once

#include "Object.h"
#include "Allocators/LinearAllocator.h"
#include <cassert>

class HybridMarkAndCopyGC : public GarbageCollector
{
public:
    std::unordered_set<LocalSpaceObject*> m_Allocated;
    std::unordered_set<Object*> m_Visited;
    size_t m_GCInterval;
    size_t m_AllocsSinceGC = 0;
    size_t m_NumberOfSpaces;
    size_t m_CurrentFromSpaceIndex;
    std::vector<LocalSpaceObject**> m_Roots;
    void* m_AllocatedSpace;
    size_t m_LocalSpaceSize;

public:
    HybridMarkAndCopyGC(const size_t gcInterval, const size_t localSpaceSize, const size_t numberOfSpaces = 8)
        : m_GCInterval(gcInterval),
          m_LocalSpaceSize(localSpaceSize),
          m_NumberOfSpaces(numberOfSpaces)
    {

    }

    void* Allocate(const size_t size) override
    {
        if (++m_AllocsSinceGC == m_GCInterval)
        {
            CollectGarbage();
            m_AllocsSinceGC = 0;
        }
        auto memory = ::operator new(size);
        m_Allocated.insert(static_cast<LocalSpaceObject*>(memory));
        return memory;
    }

    void AddRoot(LocalSpaceObject** root)
    {
        std::vector<bool> localSpacesWithRoots;
        localSpacesWithRoots.resize(m_NumberOfSpaces);
        size_t numberOfSpacesWithRoots = 0;
        
        for (const auto& setRoots : m_Roots)
        {
            if (!localSpacesWithRoots[(*setRoots)->m_LocalSpaceIndex])
            {
                ++numberOfSpacesWithRoots;
            }

            localSpacesWithRoots[(*setRoots)->m_LocalSpaceIndex] = true;
        }

        assert(numberOfSpacesWithRoots < m_NumberOfSpaces);

        if (numberOfSpacesWithRoots == m_NumberOfSpaces - 1)
        {
            const size_t lastRootLocalSpaceIndex = (*m_Roots[m_Roots.size() - 1])->m_LocalSpaceIndex;
            (*root)->m_LocalSpaceIndex = lastRootLocalSpaceIndex;
            m_Roots.push_back(root);
            return;
        }

        for (size_t i = 0; i < m_NumberOfSpaces; ++i)
        {
            if (!localSpacesWithRoots[i] && m_CurrentFromSpaceIndex != i)
            {
                (*root)->m_LocalSpaceIndex = i;
                m_Roots.push_back(root);
                return;
            }
        }
    }

    void CollectGarbage()
    {
        m_Visited.clear();

        // TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
        //if (m_Root == nullptr)
        //{
        //    return;
        //}

        for (const auto& root : m_Roots)
        {
            (*root)->VisitReferences(this, nullptr, m_Visited);
        }

        std::unordered_set<LocalSpaceObject*> visitedFromSpaceObjects;

        for (auto& visited : m_Visited)
        {
            LocalSpaceObject* localSpaceVisitedObject = (static_cast<LocalSpaceObject*>(visited));
            if ((static_cast<LocalSpaceObject*>(visited))->m_LocalSpaceIndex == m_CurrentFromSpaceIndex)
            {
                visitedFromSpaceObjects.insert(localSpaceVisitedObject);
            }
        }

        for (const auto object : m_Allocated)
        {
            if (m_Visited.find(object) == m_Visited.end())
            {
                object->~LocalSpaceObject();
                ::operator delete(object);
            }
        }

        //m_Allocated = m_Visited;
    }

    void Shutdown() override
    {
        for (const auto object : m_Allocated)
        {
            object->~LocalSpaceObject();
            ::operator delete(object);
        }
    }

    void FlipSpaces()
    {
        const size_t toSpaceIndex = (m_CurrentFromSpaceIndex + 1) % m_NumberOfSpaces;

        std::vector<LocalSpaceObject**> rootsInToSpace;

        for (const auto& root : m_Roots)
        {
            if ((*root)->m_LocalSpaceIndex == toSpaceIndex)
            {
                rootsInToSpace.push_back(root);
            }
        }

        LinearAllocator toSpace =
            LinearAllocator(static_cast<unsigned char*>(m_AllocatedSpace) + (m_LocalSpaceSize * toSpaceIndex),
                m_LocalSpaceSize);

        for (auto& root : rootsInToSpace)
        {
            Copy(*root, toSpace);
        }
    }

    void Copy(LocalSpaceObject*& objectToCopy, LinearAllocator& toSpaceAllocator)
    {
        if (objectToCopy->m_ForwardPointer != nullptr)
        {
            return;
        }

        objectToCopy->m_ForwardPointer = static_cast<Object*>(toSpaceAllocator.Allocate(objectToCopy->m_Size));
        std::memcpy(objectToCopy->m_ForwardPointer, objectToCopy, objectToCopy->m_Size);

        for (size_t i = 0; i < objectToCopy->m_NumberOfReferredObjects; ++i)
        {
            LocalSpaceObject* adjacentLocalSpaceObject = static_cast<LocalSpaceObject*>(objectToCopy->m_ReferedObjects[i]);
            Copy(adjacentLocalSpaceObject, toSpaceAllocator);
            objectToCopy->m_ReferedObjects[i] = objectToCopy->m_ForwardPointer;
        }
    }
};


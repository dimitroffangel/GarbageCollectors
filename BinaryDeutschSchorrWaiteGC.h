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
    BinaryDeutschSchorrWaiteGC(const size_t gcInterval);

    void* Allocate(const size_t size) override;

    void CollectGarbage();

    void VisitReference(Object* from, Object** to, void* state) override;

    void SetRoot(Object** root) override
    {

    }

    void SetRoot(BDSW_Binary_Object** root)
    {
        m_Root = root;
    }

    void Shutdown() override;
};


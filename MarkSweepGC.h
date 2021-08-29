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
    MarkSweepGC(const size_t gcInterval);

    void* Allocate(const size_t size) override;

    void SetRoot(Object** root)
    {
        m_Root = root;
    }

    void CollectGarbage();

    void Shutdown() override;
};


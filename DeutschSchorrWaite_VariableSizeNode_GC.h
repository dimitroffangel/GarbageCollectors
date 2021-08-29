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
    DeutschSchorrWaite_VariableSizeNode_GC(const size_t gcInterval);

    void* Allocate(const size_t size) override;

    void CollectGarbage();

    // Inherited via GarbageCollector
    void VisitReference(Object* from, Object** to, void* state) override;
    void SetRoot(Object** root) override;

    void SetRoot(DeutschSchorrWaite_VariableSizeNode** root)
    {
        m_Root = root;
    }

    void Shutdown() override;
};


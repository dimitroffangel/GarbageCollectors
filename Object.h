#pragma once

#include <cstddef>
#include <unordered_set>
#include <memory>

class ObjectVisitor;
class BDSW_ObjectVisitor;

class Object
{
public:
    int m_NumberOfReferredObjects;
    Object** m_ReferedObjects;

public:
    virtual ~Object() {}
    virtual void VisitReferences(ObjectVisitor* visitor, void* state) = 0;
};

class BDSW_Binary_Object
{
public:
    bool m_HasMarkedObject;
    BDSW_Binary_Object** m_ReferedObjects;
    bool m_HasIteratedLeftChild;

    virtual ~BDSW_Binary_Object() {}
};

class DeutschSchorrWaite_VariableSizeNode
{
public:
    bool m_HasMarkedObject;
    DeutschSchorrWaite_VariableSizeNode** m_ReferedObjects;
    int m_CurrentIteratedChild = 0;
    size_t m_NumberOfChildren;

    virtual ~DeutschSchorrWaite_VariableSizeNode() {}
};

class ObjectVisitor
{
public:
    virtual ~ObjectVisitor() {}

    // Object** to allows the reference to be moved
    virtual void VisitReference(Object* from, Object** to, void* state) = 0;
};

class GarbageCollector : public ObjectVisitor
{
public:
    virtual void* Allocate(size_t size) = 0;

    // Object** to allows the root to be moved
    virtual void SetRoot(Object** root) = 0;

    virtual void Shutdown() = 0;
};

class DestroyVisitor : public ObjectVisitor
{
public:
    std::unordered_set<Object*> m_Visited;

public:
    virtual void Destroy(Object* o)
    {
        auto insertResult = m_Visited.insert(o);
        if (insertResult.second)
        {
            o->VisitReferences(this, nullptr);
            o->~Object();
        }
    }

    void VisitReference(Object*, Object** to, void*) override
    {
        if (*to)
        {
            Destroy(*to);
        }
    }
};

std::unique_ptr<GarbageCollector> CreateGarbageCollector(int argc, char* argv[]);

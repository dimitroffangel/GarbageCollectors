#include "BoehmDemersWeiserGC.h"



BoehmDemersWeiserGC::BoehmDemersWeiserGC(const size_t gcInterval)
	:m_GCInterval(gcInterval)
{

}

void* BoehmDemersWeiserGC::Allocate(size_t size)
{
	Object* const  allocatedObject = static_cast<Object*>(::operator new(size));
	m_Allocated.insert(allocatedObject);

	return allocatedObject;
}

void BoehmDemersWeiserGC::Shutdown()
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
			object->~Object();
			::operator delete(object, sizeof(object));
		}
	}

	m_Allocated = m_Visited;
}

void BoehmDemersWeiserGC::CollectGarbage()
{
	m_Visited.clear();

	// TODO:: what happens if  m_root == nullptr and m_AllocsSinceGC > 0 
	if (m_Root == nullptr)
	{
		return;
	}

	m_Visited.insert(*m_Root);
	(*m_Root)->VisitReferences(this, nullptr, m_Visited);

	for (const auto& object : m_Allocated)
	{
		if (m_Visited.find(object) == m_Visited.end())
		{
			object->~Object();
			::operator delete(object, sizeof(object));
		}
	}

	// compact the region


	m_Allocated = m_Visited;
}
#include "HybridMarkAndCopyGC.h"

void HybridMarkAndCopyGC::VisitReference(Object* from, Object** to, void* state)
{
	const auto insert = m_Visited.insert(*to);
	if (insert.second)
	{
		(*to)->VisitReferences(this, state);
	}

}

void HybridMarkAndCopyGC::SetRoot(Object** root)
{
}

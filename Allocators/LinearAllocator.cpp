#include "LinearAllocator.h"

#include <iostream>

LinearAllocator::LinearAllocator(void* const memoryAllocated, const size_t maxSize)
	:	m_InitialLocation(m_AvailableBlock),
		m_AvailableBlock(m_AvailableBlock),
		m_MaximumSize(maxSize)
{

}

LinearAllocator::LinearAllocator(const size_t sizeToAllocate)
	:m_MaximumSize(sizeToAllocate)
{
	m_InitialLocation = m_AvailableBlock = new unsigned char[sizeToAllocate];
}

void LinearAllocator::Reset()
{
	m_AvailableBlock = m_InitialLocation;
	m_CurrentTakenSize = 0;
}

void* LinearAllocator::Allocate(const size_t blockToAllocate)
{
	if (m_CurrentTakenSize + blockToAllocate > m_MaximumSize)
	{
		std::cout << "LinearAllocator::Allocate(const size_T blockToAllocate m_CurrentTakenSize + blockToAllocate > m_MaximumSize" << '\n';

		return nullptr;
	}

	void* const freeBlock = m_AvailableBlock;
	
	m_AvailableBlock = static_cast<unsigned char*>(m_AvailableBlock) + blockToAllocate;
	m_CurrentTakenSize += blockToAllocate;


	return freeBlock;
}
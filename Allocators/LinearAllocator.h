#pragma once

class LinearAllocator
{
public:
	void* m_InitialLocation;
	void* m_AvailableBlock;
	size_t m_MaximumSize = 1024;
	size_t m_CurrentTakenSize = 0;

	LinearAllocator(void* const memoryAllocated, const size_t maxSize);

	LinearAllocator(const size_t sizeToAllocate = 1024);

	void Reset();

	void* Allocate(const size_t blockToAllocate);
	
	//void Deallocate();
};


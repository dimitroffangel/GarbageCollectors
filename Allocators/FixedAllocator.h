#ifndef FIXEDALLOCATOR_H_GUARD
#define FIXEDALLOCATOR_H_GUARD

#include <vector>

#include "Chunk.h"

class FixedAllocator
{
private:
	void DoDeallocation(void* pointer);
	Chunk* FindChunkWithPointer(void* pointer);

	using Chunks = std::vector<Chunk>;

	std::size_t m_BlockSize;
	unsigned char m_NumberOfBlocks;

	Chunks m_Chunks;
	Chunk* m_RecentlyAllocatedChunk = nullptr;
	Chunk* m_RecentlyDeallocatedChunk = nullptr;

	FixedAllocator* m_PreviousFixedAlloc = nullptr;
	FixedAllocator* m_NextFixedAlloc = nullptr;

public:
	explicit FixedAllocator(std::size_t blockSize = 0);
	~FixedAllocator();

public:
	FixedAllocator(const FixedAllocator&);
	FixedAllocator& operator=(const FixedAllocator&);

	void* Allocate();
	void Deallocate(void* pointer);

	std::size_t BlockSize() const
	{
		return m_BlockSize;
	}
};

#endif

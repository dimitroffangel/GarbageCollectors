#include <climits>

#ifndef FIXEDALLOCATOR_H_GUARD
#define FIXEDALLOCATOR_H_GUARD

const size_t DEFAULT_CHUNK_SIZE = 4096;

const size_t UNSIGNED_CHAR_MAX = UCHAR_MAX;

const unsigned DefaultNumberOfBlocks = 8;

const size_t NUMBER_OF_UNSIGNED_CHARS = 4;
const size_t NUMBER_OF_POINTERS = 2;
const size_t SIZE_OF_CHUNK_INFO_NEEDED = 2 * sizeof(unsigned char);


class FixedLocationAllocator
{
public:
	FixedLocationAllocator(void*, const unsigned char, const size_t, const unsigned char);

	FixedLocationAllocator(const FixedLocationAllocator&) = delete;
	FixedLocationAllocator& operator=(const FixedLocationAllocator&) = delete;

public:
	void* Allocate();
	void Deallocate(void* pointer);

private:
	unsigned char* m_PointerToData;
	//Chunk* m_RecentlyAllocatedChunk;
	//Chunk* m_RecentlyDeallocatedChunk;

	inline void InitializeChunk(void* chunkPointerToData, const size_t numberOfBlockss);
	inline void* ChunkAllocation(const size_t chunkIndex);
	inline void ChunkDeallocation(void* pointerToFree, const size_t chunkIndex);
	void* FindChunkWithPointer(void* pointerToFind);
	void DoDeallocation(void* pointer);
};


#endif
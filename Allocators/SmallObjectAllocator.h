#ifndef SMALLOBJECTALLOCATOR_H_GUARD
#define SMALLOBJECTALLOCATOR_H_GUARD

#include <vector>

#include "FixedAllocator.h"

const int MAX_SMALL_OBJECT_SIZE = 64;

class SmallObjectAllocator
{
public:
	static void Initialize();

	static void Initialize(const int chunkSize, const int maxSmallObjectSize);

	static void Shutdown();

public:
	SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize);

	void* Allocate(std::size_t numberOfBytes);
	void Deallocate(void* pointer, std::size_t size);

	SmallObjectAllocator(const SmallObjectAllocator&) = delete;
	SmallObjectAllocator operator=(const SmallObjectAllocator&) = delete;

	using Pool = std::vector<FixedAllocator>;
	Pool m_Pool;
	FixedAllocator* m_PointerToLastAllocator = nullptr;
	FixedAllocator* m_PointerToLastDeallocator = nullptr;
	std::size_t m_ChunkSize;
	std::size_t m_MaxObjectSize;
};

extern SmallObjectAllocator* g_SmallObjectAllocator;

#endif
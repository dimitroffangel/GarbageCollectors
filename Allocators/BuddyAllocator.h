#ifndef BUDDYALLOCATOR_H_GUARD
#define BUDDYALLOCATOR_H_GUARD

#include "FastLogarithm.h"

#include <iostream>

/*
	totalSize = (1 << numberOfLevels) * leafSize
	sizeOfEachBlockOnLevel(n) = totalSize / (1 << n)
	maxBlocksOfLevel(n) = (1 << n)
	(1 << level) + indexInLevel - 1 -> unqueue index in the buddyAllocator


	(1 << numberOfLevels) - 1 - number of indices
*/

const PtrInt DEFAULT_BUDDY_ALLOCATOR_SIZE = PtrInt(1) << 24;
const PtrInt LEAF_SIZE = PtrInt(1) << 5;

struct FreeListInformation
{
	PtrInt* previous = nullptr;
	PtrInt* next = nullptr;
};

class BuddyAllocator
{
	static const int MAX_LEVELS = 24;
	static const size_t NUMBER_OF_BITSET_FOR_FREE_TABLE = DEFAULT_BUDDY_ALLOCATOR_SIZE / LEAF_SIZE;

private:
	BuddyAllocator();
	~BuddyAllocator();
	BuddyAllocator(const BuddyAllocator& other) = delete;
	BuddyAllocator& operator=(const BuddyAllocator& rhs) = delete;

public:
	static void Initialize();
	static void Shutdown();

private:
	void SimulateAllocationForLeaves_ForFreeList(size_t numberOfAllocationsOnLeafsNeeded);
	inline void SetBitToOne_FreeTable(const size_t parentIndex);
	inline void SetBitToZero_FreeTable(const size_t parentIndex);
	inline bool GetBitFromFreeTable(const size_t parentIndex);
	inline void XOR_Bit_FreeTable(const size_t parentIndex, const bool XOR_Argument);

	inline void SetBitToOne_SplitTable(const size_t parentIndex);
	inline void SetBitToZero_SplitTable(const size_t parentIndex);
	inline bool GetBitFromSplitTable(const size_t parentIndex);
	inline void XOR_Bit_SplitTable(const size_t parentIndex, const bool XOR_Argument);


public:
	void Deallocate(void* pointer, size_t blockSize);
	void Free(void* pointer, size_t levelIndex);
	void Free(void* pointer);
	void* Allocate(size_t blockSize);
	
public:
	//size_t m_NumberOfLevels = MAX_LEVELS;

	unsigned char* m_PointerToData;
	
	inline size_t GetTotalSize() 
	{
		//return (1 << m_NumberOfLevels) * LEAF_SIZE;
		return (PtrInt(1) << MAX_LEVELS);
	}

	// TotalSize / (2^levelIndex) = sizeOfEachBlockThere 
	inline size_t GetSizeOfLevel(size_t n)
	{
		return GetTotalSize() / (PtrInt(1) << n);
	}

	inline size_t GetLevel(size_t uniqueIndex)
	{
		if (uniqueIndex == 0)
		{
			return 0;
		}

		return FastLogarithm::Log2_64(uniqueIndex + 1);
	}

	inline size_t IndexInLevel(size_t uniqueIndex)
	{
		size_t levelOfIndex = GetLevel(uniqueIndex);
		size_t firstUniqueIndexOnLevel = (PtrInt(1) << levelOfIndex) - 1;

		return uniqueIndex - firstUniqueIndexOnLevel;
	}

	inline size_t IndexInLevelOf(void* pointer, size_t levelIndex)
	{
		// pointer substraction
		if (!((uintptr_t)(pointer) >= (uintptr_t)(m_PointerToData) &&
			(uintptr_t)(pointer) < (uintptr_t)(m_PointerToData)+(uintptr_t)(GetTotalSize())))
		{
			std::cerr << "BuddyAllocator()::IndexInLevelOf(void*, size_t) pointer was out of range" << '\n';
			return 0;
		}

		unsigned char* getUnsignedCharPointer = static_cast<unsigned char*>(pointer);

		size_t res = getUnsignedCharPointer - m_PointerToData;

		return (getUnsignedCharPointer - m_PointerToData) / GetSizeOfLevel(levelIndex);
	}

	inline size_t GetUniqueIndex(void* pointer, size_t levelIndex)
	{
		/*
			indexInLevelOfThePointer = uniqueIndex - firstUniqueIndexOnLevel
			<=>
			uniqueIndex = indexInLevelOfThePointer + firstUniqueIndexOnLevel
		*/ 
		
		size_t firstUniqueIndexOnLevel = (PtrInt(1) << levelIndex) - 1;
	
		size_t indexInLevelOfThePointer = IndexInLevelOf(pointer, levelIndex);
	
		return indexInLevelOfThePointer + firstUniqueIndexOnLevel;
	}

	inline size_t GetParent(size_t uniqueIndex)
	{
		if (uniqueIndex == 0)
		{
			return 0;
		}

		if (uniqueIndex % 2 == 0)
		{
			return (uniqueIndex - 1) / 2;
		}

		return uniqueIndex / 2;
	}

	inline size_t GetUniqueIndex(size_t localIndexInLevel, size_t levelIndex)
	{
		size_t firstUniqueIndexInLevel = (PtrInt(1) << levelIndex) - 1;
		
		return localIndexInLevel + firstUniqueIndexInLevel;
	}

	inline size_t GetBuddy(void* pointer, size_t levelIndex)
	{
		size_t indexInLevelOfPointer = IndexInLevelOf(pointer, levelIndex);

		if (indexInLevelOfPointer % 2 == 0)
		{
			return indexInLevelOfPointer + 1;
		}

		return indexInLevelOfPointer - 1;
	}
}; 

extern BuddyAllocator* g_BuddyAllocator;

#endif

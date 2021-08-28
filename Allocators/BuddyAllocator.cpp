#include "BuddyAllocator.h"

#include <iostream>
#include <assert.h>

#include "SmallObject.h"

void BuddyAllocator::SimulateAllocationForLeaves_ForFreeList(size_t numberOfAllocationsOnLeafsNeeded)
{
	int level = MAX_LEVELS - FastLogarithm::Log2_64(LEAF_SIZE);

	while (numberOfAllocationsOnLeafsNeeded > 0 && level >= 0)
	{
		for (size_t i = 0; i < numberOfAllocationsOnLeafsNeeded; i += 2)
		{
			size_t levleSize = GetSizeOfLevel(level);
			size_t uniqueIndex = GetUniqueIndex(m_PointerToData + (i * GetSizeOfLevel(level)), level);
			size_t parentIndex = GetParent(uniqueIndex);

			SetBitToOne_SplitTable(parentIndex);
			//m_SplitTable[parentIndex] = 1;

			if (i % 2 == 0 && i + 1 < numberOfAllocationsOnLeafsNeeded)
			{
				// the buddy of the leaf is taken as well
				SetBitToZero_FreeTable(parentIndex);
				//m_FreeTable[parentIndex] = 0;
			}

			else
			{
				// the buddy is free
				SetBitToOne_FreeTable(parentIndex);
				//m_FreeTable[parentIndex] = 1;
			}
		}

		if (numberOfAllocationsOnLeafsNeeded % 2 != 0 && level != 0)
		{
			// there is a freeSlot there
			*(PtrInt*)(m_PointerToData + sizeof(PtrInt) * level) =
				(PtrInt)(m_PointerToData + (GetSizeOfLevel(level) * numberOfAllocationsOnLeafsNeeded));

			//PtrInt* fooProdigy = (PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level));
			//FreeListInformation* fooProdigyInfo = (FreeListInformation*)(*fooProdigy);

			*((FreeListInformation*)(m_PointerToData + (GetSizeOfLevel(level) * numberOfAllocationsOnLeafsNeeded))) = { nullptr, nullptr };
 		}

		numberOfAllocationsOnLeafsNeeded = numberOfAllocationsOnLeafsNeeded / 2 + !(numberOfAllocationsOnLeafsNeeded % 2 == 0);
			
		--level;
	}
}

void BuddyAllocator::SetBitToOne_FreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	PtrInt tref = (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
	
	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) |= (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res = GetBitFromFreeTable(parentIndex);

	assert((*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8))));
	assert(GetBitFromFreeTable(parentIndex));
}

void BuddyAllocator::SetBitToZero_FreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) &= ~(PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res = GetBitFromFreeTable(parentIndex);


	assert(!((*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)))));
	assert(!GetBitFromFreeTable(parentIndex));
}

inline bool BuddyAllocator::GetBitFromFreeTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8));

	//bool res = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	return (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
}

inline void BuddyAllocator::XOR_Bit_FreeTable(const size_t parentIndex, const bool XOR_Argument)
{
	bool result = GetBitFromFreeTable(parentIndex) ^ XOR_Argument;
	
	if (result == 0)
	{
		SetBitToZero_FreeTable(parentIndex);
	}

	else
	{
		SetBitToOne_FreeTable(parentIndex);
	}

	bool res = GetBitFromFreeTable(parentIndex);
}

void BuddyAllocator::SetBitToOne_SplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) |= (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res = GetBitFromSplitTable(parentIndex);

	assert((*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8))));
	assert(GetBitFromSplitTable(parentIndex));
}

void BuddyAllocator::SetBitToZero_SplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	//bool test = *static_cast<PtrInt*>(address);
	bool res1 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	(*static_cast<PtrInt*>(address)) &= ~(PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res2 = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	//bool res = GetBitFromSplitTable(parentIndex);

	assert(!((*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)))));
	assert(!GetBitFromSplitTable(parentIndex));
}

inline bool BuddyAllocator::GetBitFromSplitTable(const size_t parentIndex)
{
	const int highestLevel = MAX_LEVELS;

	void* address = (m_PointerToData + ((highestLevel) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
		(sizeof(PtrInt) * ((parentIndex / sizeof(PtrInt)) / 8)));

	//bool res = (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));

	return (*static_cast<PtrInt*>(address)) & (PtrInt(1) << (parentIndex % (sizeof(PtrInt) * 8)));
}

inline void BuddyAllocator::XOR_Bit_SplitTable(const size_t parentIndex, const bool XOR_Argument)
{
	bool result = GetBitFromSplitTable(parentIndex) ^ XOR_Argument;

	if (result == 0)
	{
		SetBitToZero_SplitTable(parentIndex);
	}

	else
	{
		SetBitToOne_SplitTable(parentIndex);
	}

	bool res = GetBitFromSplitTable(parentIndex);
}

BuddyAllocator::BuddyAllocator()
{
	m_PointerToData = new unsigned char[DEFAULT_BUDDY_ALLOCATOR_SIZE];

	//m_FreeLists[0] = m_PointerToData;
	//*static_cast<FreeListInformation*>(m_FreeLists[0]) = { nullptr, nullptr };

	//for (size_t i = 1; i < MAX_LEVELS; ++i)
	//{
	//	m_FreeLists[i] = nullptr;
	//}
	//{
	//	void* startOfTheListInformationPointers = Allocate(LEAF_SIZE);

	//	if (startOfTheListInformationPointers == nullptr)
	//	{
	//		std::cerr << "BuddyAllocator::Initialize startOfTheListInformationPointers == nullptr" << '\n';
	//		return;
	//	}

	//	*static_cast<PtrInt*>(startOfTheListInformationPointers) = (PtrInt)(nullptr);
	//}

	{
		*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);

		*((FreeListInformation*)(m_PointerToData)) = { nullptr, nullptr };
	}

	size_t size = sizeof(PtrInt);

	for (size_t i = 1; i < MAX_LEVELS; ++i)
	{
		*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * i)) = 0;

		size += sizeof(PtrInt);
	}

	for (size_t i = 0; i < NUMBER_OF_BITSET_FOR_FREE_TABLE; i+=sizeof(PtrInt)*8)
	{
		void* freeTableAddress = (m_PointerToData + ((MAX_LEVELS) * sizeof(PtrInt)) + sizeof(PtrInt) * ((i / sizeof(PtrInt)) / 8));
		void* splitTableAddress = (m_PointerToData + ((MAX_LEVELS) * sizeof(PtrInt)) + (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8) +
			(sizeof(PtrInt) * ((i / sizeof(PtrInt)) / 8)));

		*static_cast<PtrInt*>(freeTableAddress) = 0;
		*static_cast<PtrInt*>(splitTableAddress) = 0;

		//SetBitToZero_FreeTable(i);
		//SetBitToZero_SplitTable(i);
	}

	//for (size_t i = 0; i < NUMBER_OF_BITSET_FOR_FREE_TABLE; ++i)
	//{
	//	SetBitToZero_FreeTable(i);
	//	SetBitToZero_SplitTable(i);
	//}

	size = size + (2 * NUMBER_OF_BITSET_FOR_FREE_TABLE / 8);

	size_t numberOfAllocationsOnLeafsNeeded =
		(size) / LEAF_SIZE + !(size % LEAF_SIZE == 0);

	SimulateAllocationForLeaves_ForFreeList(numberOfAllocationsOnLeafsNeeded);

}

BuddyAllocator::~BuddyAllocator()
{
	delete[] m_PointerToData;
}

BuddyAllocator* g_BuddyAllocator = nullptr;

void BuddyAllocator::Initialize()
{
	if (g_BuddyAllocator != nullptr)
	{
		return;
	}

	g_BuddyAllocator = new BuddyAllocator();
}

void BuddyAllocator::Shutdown()
{
	delete g_BuddyAllocator;
	g_BuddyAllocator = nullptr;
}

void BuddyAllocator::Deallocate(void* pointer, size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		g_SmallObjectAllocator->Deallocate(pointer, blockSize);

		return;
	}

	size_t level = MAX_LEVELS - FastLogarithm::Log2_64(blockSize) - !FastOperationsWithTwo::IsPowerOfTwo(blockSize);
	Free(pointer, level);
}

void BuddyAllocator::Free(void* pointerToFree, size_t levelIndex)
{
	if (pointerToFree == nullptr)
	{
		return;
	}

	// validate that the pointerToFree and level are correct

	if (levelIndex == 0)
	{
		*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);
		//*(FreeListInformation*)(m_PointerToData) = { nullptr, nullptr };

		//m_FreeLists[0] = m_PointerToData;
		return;
	}

	// the pointer may be to a location, which is split, not used
	bool a = uintptr_t(pointerToFree) >= (uintptr_t)(m_PointerToData);
	bool b = (uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+
		(uintptr_t)(((MAX_LEVELS) * sizeof(PtrInt)) + 2 * (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8));

	if ((uintptr_t(pointerToFree) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+
		(uintptr_t)(((MAX_LEVELS) * sizeof(PtrInt)) + 2 * (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8))))
	{
		std::cout << "BuddyAllocator::Free(void*, size_t) address that was tasked with freeing was allocator specific memory and therefore not freeable" << '\n';
		return;
	}

	// check if the level is right
	if (levelIndex >= MAX_LEVELS)
	{
		std::cerr << "BuddyAllocator::Free(void*, size_t) levelIndex >= MAX_LEVELS" << '\n';
		return;
	}

	size_t uniqueIndexOfThePointer = GetUniqueIndex(pointerToFree, levelIndex);
	
	// check if the level points 
	if (uniqueIndexOfThePointer < NUMBER_OF_BITSET_FOR_FREE_TABLE && GetBitFromSplitTable(uniqueIndexOfThePointer) == 1)
	{
		std::cerr << "BuddyAllocator::Free(void* pointerToFree, size_t levelIndex) m_SplitTable[uniqueIndexOfThePointer] = 1" << '\n';
		return;
	}

	// check pointer alignment
	if (m_PointerToData + (GetSizeOfLevel(levelIndex) * IndexInLevel(uniqueIndexOfThePointer)) != pointerToFree)
	{
		std::cerr << "BuddyAllocator()::Free(void*, size_t) (PtrInt)(pointer) % sizeOfLevel != 0 " << '\n';
		return;
	}
	
	size_t buddyIndexOfThePointerInLevel = GetBuddy(pointerToFree, levelIndex);
	size_t buddyIndexOfThePointer = GetUniqueIndex(buddyIndexOfThePointerInLevel, levelIndex);
	size_t getParentIndex = GetParent(uniqueIndexOfThePointer);
	size_t parentLevel = levelIndex - 1;
	

	while (true)
	{
		// the other buddy is free
		if (GetBitFromFreeTable(getParentIndex) == 1)
		{
			// change the m_FreeTable
			SetBitToZero_FreeTable(getParentIndex);
 			//m_FreeTable[getParentIndex] = 0;

			// add the free slot to the parentLevel of freeSlots

			// declare the two pointers
			// set the slot of the released pointer to 0
			PtrInt* releasedPointer = static_cast<PtrInt*>(pointerToFree);
			*releasedPointer = (PtrInt)(nullptr);

			// -----reorganize the chaining on the level of the releasedPointer
			// get the buddy pointer that is about to be released
			void* buddyPointer = (m_PointerToData + (GetSizeOfLevel(levelIndex) * buddyIndexOfThePointerInLevel));
			
			// set the freeList on that level with value in the pointer
			if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) == (PtrInt)(buddyPointer))
			{
				FreeListInformation* freeSlot = static_cast<FreeListInformation*>(buddyPointer);

				*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) = (PtrInt)(freeSlot->next);

				//m_FreeLists[levelIndex] = static_cast<FreeListInformation*>(buddyPointer)->next;
			}

			else
			{
				FreeListInformation* buddyFreeList = static_cast<FreeListInformation*>(buddyPointer);

				((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->previous))->next
					= static_cast<FreeListInformation*>(buddyPointer)->next;

				if (buddyFreeList->next != nullptr)
				{
					((FreeListInformation*)(static_cast<FreeListInformation*>(buddyPointer)->next))->previous
						= static_cast<FreeListInformation*>(buddyPointer)->previous;
				}
				
				else
				{
					int a = 42;
				}
			}

			levelIndex = levelIndex - 1;

			// set the splitTable value of the parent block to 0
			{
				assert(GetBitFromSplitTable(getParentIndex) == 1);
				SetBitToZero_SplitTable(getParentIndex);

				//assert(m_SplitTable[getParentIndex] == 1);
				//m_SplitTable[getParentIndex] = 0;
			}

			// if has reached top, abort the releasing of blocks
			if (levelIndex == 0)
			{
				*((PtrInt*)(m_PointerToData)) = (PtrInt)(m_PointerToData);

				//*(FreeListInformation*)(m_PointerToData) = { nullptr, nullptr };
				//m_FreeLists[0] = m_PointerToData;
				return;
			}

			// find the one with odd uniqueIndex
			// NOTE:::->   levelIndex > 0 => has a buddy

			if (uniqueIndexOfThePointer % 2 == 0)
			{
				std::swap(pointerToFree, buddyPointer);
			}

			// the parent address starts with the address of the current pointer
			uniqueIndexOfThePointer = GetUniqueIndex(pointerToFree, levelIndex);

			buddyIndexOfThePointerInLevel = GetBuddy(pointerToFree, levelIndex);
			buddyIndexOfThePointer = GetUniqueIndex(buddyIndexOfThePointerInLevel, levelIndex);
			getParentIndex = GetParent(uniqueIndexOfThePointer);
			parentLevel = levelIndex - 1;
		}

		else
		{
			// change the m_FreeTable
			SetBitToOne_FreeTable(getParentIndex);
			//m_FreeTable[getParentIndex] = 1;

			// set the head of the linkedList to the pointerToFree location
			if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex))) == 0)
			{
				//m_FreeLists[levelIndex] = pointerToFree;

				static_cast<FreeListInformation*>(pointerToFree)->next = nullptr;
				static_cast<FreeListInformation*>(pointerToFree)->previous = nullptr;
			}

			else
			{
				((FreeListInformation*)(*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * levelIndex)))))->previous =
					static_cast<PtrInt*>(pointerToFree);

				static_cast<FreeListInformation*>(pointerToFree)->next = (PtrInt*)(*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * levelIndex)));
				static_cast<FreeListInformation*>(pointerToFree)->previous = nullptr;
			}

			*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * levelIndex)) = (PtrInt)(pointerToFree);
			//m_FreeLists[levelIndex] = pointerToFree;

			// the other buddy is not free, so there cannot be going forward
			return;
		}
	}
}

void BuddyAllocator::Free(void* pointerToFree)
{
	if (pointerToFree == nullptr)
	{
		return;
	}

	// the pointer may be to a location, which is split, not used
	bool a = uintptr_t(pointerToFree) >= (uintptr_t)(m_PointerToData);
	bool b = (uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+
		(uintptr_t)(((MAX_LEVELS) * sizeof(PtrInt)) + 2 * (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8));

	if ((uintptr_t(pointerToFree) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+
		(uintptr_t)(((MAX_LEVELS) * sizeof(PtrInt)) + 2 * (NUMBER_OF_BITSET_FOR_FREE_TABLE / 8))))
	{
		//std::cout << "BuddyAllocator::Free(void*) address that was tasked with freeing was allocator specific memory and therefore not freeable" << '\n';
		return;
	}

	if (!((uintptr_t)(pointerToFree) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+(uintptr_t)(GetTotalSize())))
	{
		//std::cerr << "BuddyAllocator::Free(void*) pointer was out of range" << '\n';
		return;
	}

	int initialLevel = 0;

	const size_t highestLevel = MAX_LEVELS - FastLogarithm::Log2_64(LEAF_SIZE);

	while (initialLevel < highestLevel)
	{
		size_t uniqueIndex = GetUniqueIndex(pointerToFree, initialLevel);

		if (GetBitFromSplitTable(uniqueIndex) == 1)
		{
			++initialLevel;

			if (initialLevel == highestLevel)
			{
				break;
			}
		}

		else
		{
			size_t parentIndex = GetParent(uniqueIndex);
			assert(GetBitFromSplitTable(parentIndex) == 1);
			break;
		}
	}

	Free(pointerToFree, initialLevel);
}

void* BuddyAllocator::Allocate(size_t blockSize)
{
	if (blockSize < LEAF_SIZE)
	{
		//std::cout << "Size is less than the minimum..." << '\n';
		return g_SmallObjectAllocator->Allocate(blockSize);
	}

	if (blockSize > DEFAULT_BUDDY_ALLOCATOR_SIZE)
	{
		//std::cout << "Size is bigger than the allocated block..." << '\n';
		return operator new(blockSize);
	}

	size_t initialLevel = MAX_LEVELS - FastLogarithm::Log2_64(blockSize) - !FastOperationsWithTwo::IsPowerOfTwo(blockSize);

	size_t levelSize = GetSizeOfLevel(initialLevel);
	int level = initialLevel;

	// find a free block while there is a free block
	while (true)
	{
		// if there is a free block in the current level, split it on two or if it is the initial level end the inspection
		if (*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level))) != 0)
		{
			// get the raw free location
			void* rawFreeSlot = (void*)(*((PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level))));

			FreeListInformation* freeSlot = static_cast<FreeListInformation*>(rawFreeSlot);

			// change the m_FreeTable on the parent
			{
				size_t uniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

				if (uniqueIndexOfTheFreeSlot != 0)
				{
					size_t indexOfParent = GetParent(uniqueIndexOfTheFreeSlot);

					XOR_Bit_FreeTable(indexOfParent, 1);
					//m_FreeTable[indexOfParent] = 1 ^ m_FreeTable[indexOfParent];
				}
			}

			// set the level pointer to the next free location
			{
				PtrInt* levelPointer = (PtrInt*)(m_PointerToData + (sizeof(PtrInt) * level));
				*(levelPointer) = (PtrInt)(freeSlot->next);
				PtrInt res = PtrInt(freeSlot->next);
			}

			// has acquired a free block on the expected spot, return the free location
			if (level == initialLevel)
			{
				return rawFreeSlot;
			}

			//  has found a way to partition, start going down again
			else
			{
				// change the split table for the newly divided block
				{
					size_t uniqueIndexOfTheFreeSlot = GetUniqueIndex(freeSlot, level);

					assert(level <= MAX_LEVELS - FastLogarithm::Log2_64(LEAF_SIZE));
					assert(GetBitFromSplitTable(uniqueIndexOfTheFreeSlot) == 0);

					SetBitToOne_SplitTable(uniqueIndexOfTheFreeSlot);

					//assert(m_SplitTable[uniqueIndexOfTheFreeSlot] == 0);
					//m_SplitTable[uniqueIndexOfTheFreeSlot] = 1;
				}

				void* freeSlotNext = (static_cast<unsigned char*>(rawFreeSlot) + GetSizeOfLevel(level + 1));

				// assert the newly taken location has odd uniqueIndex
				{
					size_t nextSlotFreeIndex = GetUniqueIndex(freeSlotNext, level + 1);
					assert(nextSlotFreeIndex % 2 == 0);
				}

				// add the new free location to the level that is bellow the current
				*((PtrInt*)(m_PointerToData + sizeof(PtrInt) * (level + 1))) = (PtrInt)(rawFreeSlot);
				
				// set the previous and next to the next free location
				*static_cast<FreeListInformation*>(rawFreeSlot) = { nullptr, static_cast<PtrInt*>(freeSlotNext) };
				*static_cast<FreeListInformation*>(freeSlotNext) = { static_cast<PtrInt*>(rawFreeSlot), nullptr };

				++level;
			}
		}

		else
		{
			--level;
	
			if (level == -1)
			{
				break;
			}
		}
	}

	std::cout << "BuddyAllocator::Allocate() No Free Slot available..." << '\n';
	return nullptr;
}
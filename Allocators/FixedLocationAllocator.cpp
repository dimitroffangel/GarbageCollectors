#include "FixedLocationAllocator.h"

#include <assert.h>
#include <iostream>

#include "FastLogarithm.h"

FixedLocationAllocator::FixedLocationAllocator(void* location, const unsigned char blockSize, 
	const size_t sizeOfLocation, const unsigned char numberOfChunksToAllocate)
{
	assert(location != nullptr);

	m_PointerToData = static_cast<unsigned char*>(location);

	const size_t preliminarySize = 
		NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + 2 * sizeof(PtrInt) + numberOfChunksToAllocate * (2 * sizeof(unsigned char));
	
	size_t numberOfBlocks = DEFAULT_CHUNK_SIZE / blockSize;

	// calculate the number of blocks for each chunk
	if (numberOfBlocks > UNSIGNED_CHAR_MAX)
	{
		numberOfBlocks = UNSIGNED_CHAR_MAX;
	}

	else if (numberOfBlocks == 0)
	{
		// TODO:: check this
		numberOfBlocks = DefaultNumberOfBlocks;
	}


	assert(numberOfBlocks == static_cast<unsigned char>(numberOfBlocks));
	assert(preliminarySize + numberOfBlocks * blockSize * numberOfChunksToAllocate <= sizeOfLocation);

	*m_PointerToData = blockSize;
	*(m_PointerToData + 1) = static_cast<unsigned char>(numberOfBlocks);
	*(m_PointerToData + 2) = numberOfChunksToAllocate;
	*(m_PointerToData + 3) = 0; // byte used only for alignment

	// allocate the pointers
	for (size_t i = 0; i < NUMBER_OF_POINTERS; ++i)
	{
		*(PtrInt*)(m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + i * sizeof(PtrInt)) = 0;
	}

	// allocate preamble information for the chunks
	for (size_t i = 0; i < SIZE_OF_CHUNK_INFO_NEEDED; ++i)
	{
		unsigned char* currentChunkInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
			NUMBER_OF_POINTERS * sizeof(PtrInt) + i * SIZE_OF_CHUNK_INFO_NEEDED;

		// *currentChunkInfo -> firstAvailableBlock
		*static_cast<unsigned char*>(currentChunkInfo) = 0;
		
		// *(currentChunkInfo + 1) -> m_BlocksAvailable
		*static_cast<unsigned char*>(currentChunkInfo + 1) = numberOfBlocks;
	}

	// initialize the chunk
	for (size_t i = 0; i < numberOfChunksToAllocate; ++i)
	{
		void* currentChunk = m_PointerToData + preliminarySize + i *(numberOfBlocks * blockSize);
		InitializeChunk(currentChunk, numberOfBlocks);
	}
}

void* FixedLocationAllocator::Allocate()
{
	void* rawRecentlyAllocatedChunk = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char);

	unsigned char* recentlyAllocatedChunk = (unsigned char*)(*static_cast<PtrInt*>(rawRecentlyAllocatedChunk));

	const unsigned char blockSize = *m_PointerToData;

	const unsigned char numberOfBlocks = *(m_PointerToData + 1);

	const unsigned char numberOfChunks = *(m_PointerToData + 2);

	unsigned char* firstChunkInAllocator =
		m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks;

	if (recentlyAllocatedChunk == nullptr)
	{
		unsigned char recentlyAllocatedChunkBlockAvailable;
		unsigned char recentlyAllocatedChunkIndex;


		size_t currentChunkIndex = 0;

		// find the first available chunk to use for allocation
		for (;; firstChunkInAllocator += blockSize * numberOfBlocks)
		{
			if (currentChunkIndex + 1 == *(m_PointerToData + 2))
			{
				std::cout << "FixedLocationAllocator::Allocate() No free slot available..." << '\n';
				return nullptr;
			}

			unsigned char* currentChunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
				NUMBER_OF_POINTERS * sizeof(PtrInt) + currentChunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

			const unsigned char currentIteratedChunkBlockAvailable = *(currentChunkPreambleInfo + 1);

			// found it
			if (currentIteratedChunkBlockAvailable > 0)
			{
				// set the recently allocated chunk to it
				recentlyAllocatedChunk = firstChunkInAllocator;
				*((PtrInt*)(m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char))) = (PtrInt)firstChunkInAllocator;


				recentlyAllocatedChunkBlockAvailable = currentIteratedChunkBlockAvailable;

				recentlyAllocatedChunkIndex =
					(recentlyAllocatedChunk - firstChunkInAllocator) / (PtrInt(blockSize) * PtrInt(numberOfBlocks));

				break;
			}
		}
		
		assert(recentlyAllocatedChunk != nullptr);
		assert(recentlyAllocatedChunkBlockAvailable > 0);

		return ChunkAllocation(recentlyAllocatedChunkIndex);
	}

	// assert that the pointer is in the range of the block
	assert((uintptr_t)(recentlyAllocatedChunk) >= (uintptr_t)(firstChunkInAllocator) &&
		(uintptr_t)(recentlyAllocatedChunk) < 
		(uintptr_t)(firstChunkInAllocator)+(uintptr_t)(PtrInt(numberOfChunks) * PtrInt(blockSize) * (numberOfBlocks)));

	unsigned char recentlyAllocatedChunkIndex =
		(firstChunkInAllocator - recentlyAllocatedChunk) / (PtrInt(blockSize) * PtrInt(numberOfBlocks));

	unsigned char* chunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + recentlyAllocatedChunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

	unsigned char recentlyAllocatedChunkBlockAvailable = *(chunkPreambleInfo + 1);

	if (recentlyAllocatedChunk == nullptr ||
		recentlyAllocatedChunkBlockAvailable == 0)
	{
		// logic is the thoroughly the same as above;
		size_t currentChunkIndex = 0;

		for (;; firstChunkInAllocator += blockSize * numberOfBlocks)
		{
			if (currentChunkIndex + 1 == *(m_PointerToData + 2))
			{
				std::cout << "FixedLocationAllocator::Allocate() No free slot available..." << '\n';
				return nullptr;
			}

			unsigned char* currentChunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
				NUMBER_OF_POINTERS * sizeof(PtrInt) + currentChunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

			const unsigned char currentIteratedChunkBlockAvailable = *(currentChunkPreambleInfo + 1);

			if (currentIteratedChunkBlockAvailable > 0)
			{
				recentlyAllocatedChunk = firstChunkInAllocator;
				*((PtrInt*)(m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char))) = (PtrInt)firstChunkInAllocator;

				recentlyAllocatedChunkBlockAvailable = currentIteratedChunkBlockAvailable;

				recentlyAllocatedChunkIndex =
					(firstChunkInAllocator - recentlyAllocatedChunk) / (PtrInt(blockSize) * PtrInt(numberOfBlocks));

				break;
			}
		}
	}

	assert(recentlyAllocatedChunk != nullptr);
	assert(recentlyAllocatedChunkBlockAvailable > 0);

	return ChunkAllocation(recentlyAllocatedChunkIndex);
}

void FixedLocationAllocator::Deallocate(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedAllocator::Dealloacate() is nullptr" << '\n';
		return;
	}

	void* rawRecentlyDeallocatedChunk = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + sizeof(PtrInt);

	unsigned char* recentlyDeallocatedChunk = (unsigned char*)(*static_cast<PtrInt*>(rawRecentlyDeallocatedChunk));

	// assert that the pointer is in the expected range of the block
	{
		const unsigned char blockSize = *m_PointerToData;

		const unsigned char numberOfBlocks = *(m_PointerToData + 1);

		const unsigned char numberOfChunks = *(m_PointerToData + 2);

		unsigned char* firstChunkInAllocator =
			m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
			NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks;

		// assertion the recentlyDeallocatedChunk is in the range of the adderesses where the chunks in the FixedLocationAllocator are
		assert((uintptr_t)(recentlyDeallocatedChunk) >= (uintptr_t)(firstChunkInAllocator) &&
			(uintptr_t)(recentlyDeallocatedChunk) <
			(uintptr_t)(firstChunkInAllocator)+(uintptr_t)(PtrInt(numberOfChunks) * PtrInt(blockSize) * (numberOfBlocks)));
	}

	recentlyDeallocatedChunk = static_cast<unsigned char*>(FindChunkWithPointer(pointer));

	assert(recentlyDeallocatedChunk != nullptr);

	DoDeallocation(pointer);
}

void FixedLocationAllocator::InitializeChunk(void* chunkPointerToData, const size_t numberOfBlocks)
{
	assert(chunkPointerToData != nullptr);

	const size_t blockSize = *m_PointerToData;

	unsigned char i = 0;
	unsigned char* tempPointer = static_cast<unsigned char*>(chunkPointerToData);

	for (; i != numberOfBlocks; tempPointer += blockSize)
	{
		*tempPointer = ++i;
	}
}

inline void* FixedLocationAllocator::ChunkAllocation(const size_t chunkIndex)
{	
	const size_t blockSize = *m_PointerToData;

	unsigned char* chunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + chunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

	unsigned char& firstAvailableBlock = *chunkPreambleInfo;

	unsigned char& blockAvailable = *(chunkPreambleInfo + 1);

	unsigned char* chunkPointer;
	
	const unsigned char numberOfChunks = *(m_PointerToData + 2);
	{
		chunkPointer = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
			NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks + chunkIndex;
	}

	if (blockAvailable == 0)
	{
		std::cout << "Chunk is empty cannot allocate here..." << '\n';
		return nullptr;
	}

	unsigned char* pointerResult =
		chunkPointer + (firstAvailableBlock * blockSize);

	firstAvailableBlock = *pointerResult;
	--blockAvailable;

	return pointerResult;
}

inline void FixedLocationAllocator::ChunkDeallocation(void* pointerToFree, const size_t chunkIndex)
{
	if (pointerToFree == nullptr)
	{
		std::cerr << "FixedLocationAllocator::ChunkDeallocation void* pointerToFree is nullptr" << '\n';
		return;
	}

	const size_t blockSize = *m_PointerToData;

	unsigned char* chunkPreambleInfo = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + chunkIndex * SIZE_OF_CHUNK_INFO_NEEDED;

	unsigned char& firstAvailableBlock = *chunkPreambleInfo;

	unsigned char& blockAvailable = *(chunkPreambleInfo + 1);

	if (!((uintptr_t)(pointerToFree) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(pointerToFree) < (uintptr_t)(m_PointerToData)+(uintptr_t)(blockSize * (*(m_PointerToData + 1)))))
	{
		std::cerr << "FixedLocationAllocator::ChunkDeallocation Deallocate pointer is off the range of the Chunk" << '\n';
		return;
	}

	unsigned char* toReleasePointer = static_cast<unsigned char*>(pointerToFree);

	// alignment check
	assert((toReleasePointer - m_PointerToData) % blockSize == 0);

	*toReleasePointer = firstAvailableBlock;
	firstAvailableBlock = static_cast<unsigned char>((toReleasePointer - m_PointerToData) / blockSize);

	// Truncation check
	assert(firstAvailableBlock == (toReleasePointer - m_PointerToData) / blockSize);
	++blockAvailable;
}

void* FixedLocationAllocator::FindChunkWithPointer(void* pointerToFind)
{
	if (pointerToFind == nullptr)
	{
		std::cerr << "FixedLocationAllocator::FindChunkWithPointer argument is nullptr " << '\n';
		return nullptr;
	}

	void* rawRecentlyDeallocatedChunk = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + sizeof(PtrInt);
	
	unsigned char* recentlyDeallocatedChunk = (unsigned char*)(*static_cast<PtrInt*>(rawRecentlyDeallocatedChunk));

	assert(recentlyDeallocatedChunk != nullptr);

	const unsigned char blockSize = *m_PointerToData;

	const unsigned char numberOfBlocks = *(m_PointerToData + 1);

	const unsigned char numberOfChunks = *(m_PointerToData + 2);

	const size_t chunkLength = PtrInt(numberOfBlocks) * PtrInt(blockSize);

	// TODO:: assertion that the pointer is aligned to the block of memory

	unsigned char* low = recentlyDeallocatedChunk;
	unsigned char* high = recentlyDeallocatedChunk + chunkLength;

	unsigned char* firstChunkInAllocator =
		m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks;

	assert((uintptr_t)(recentlyDeallocatedChunk) >= (uintptr_t)(firstChunkInAllocator) &&
		(uintptr_t)(recentlyDeallocatedChunk) < 
		(uintptr_t)(firstChunkInAllocator)+ (uintptr_t)(PtrInt(numberOfChunks) * PtrInt(blockSize) * PtrInt(numberOfBlocks)));

	assert((uintptr_t)(pointerToFind) >= (uintptr_t)(firstChunkInAllocator) &&
		(uintptr_t)(pointerToFind) <
		(uintptr_t)(firstChunkInAllocator)+(uintptr_t)(PtrInt(numberOfChunks) * PtrInt(blockSize) * PtrInt(numberOfBlocks)));

	unsigned char* lowBound = firstChunkInAllocator;
	unsigned char* highBound = firstChunkInAllocator + chunkLength * numberOfChunks;

	if (high == highBound)
		high = nullptr;

	for (;;)
	{
		if (low != nullptr)
		{
			if ((uintptr_t)(pointerToFind) >= (uintptr_t)(low) && (uintptr_t)(pointerToFind) < (uintptr_t)(low) + (uintptr_t)(chunkLength))
			{
				return low;
			}

			if (low == lowBound)
			{
				low = nullptr;

				if (high == nullptr)
				{
					break;
				}
			}

			else
			{
				unsigned char lowChunkIndex = (low - firstChunkInAllocator) / chunkLength;

				// low != lowBound => can move left
				low = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
					NUMBER_OF_POINTERS * sizeof(PtrInt) + 
					SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks + (lowChunkIndex - 1) * chunkLength;
			}
		}

		if (high != nullptr)
		{
			if ((uintptr_t)(pointerToFind) >= (uintptr_t)(high) && (uintptr_t)(pointerToFind) < (uintptr_t)(high) +(uintptr_t)(chunkLength))
			{
				return high;
			}

			{
				// increase the high
				unsigned char highChunkIndex = (high - firstChunkInAllocator) / chunkLength;

				high = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
					NUMBER_OF_POINTERS * sizeof(PtrInt) +
					SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks + (highChunkIndex + 1) * chunkLength;
			}

			if (high == highBound)
			{
				high = nullptr;

				if (low == nullptr)
				{
					break;
				}
			}
		}
	}

	// should not come to this
	assert(false);
	return nullptr;
}

void FixedLocationAllocator::DoDeallocation(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedLocationAllocator::DoDeallocation() pointer is nullptr" << '\n';
		return;
	}

	void* rawRecentlyDeallocatedChunk = m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) + sizeof(PtrInt);

	unsigned char* recentlyDeallocatedChunk = (unsigned char*)(*static_cast<PtrInt*>(rawRecentlyDeallocatedChunk));

	assert(recentlyDeallocatedChunk != nullptr);

	const unsigned char blockSize = *m_PointerToData;

	const unsigned char numberOfBlocks = *(m_PointerToData + 1);

	const unsigned char numberOfChunks = *(m_PointerToData + 2);

	const size_t chunkLength = numberOfBlocks * blockSize;

	unsigned char* firstChunkInAllocator =
		m_PointerToData + NUMBER_OF_UNSIGNED_CHARS * sizeof(unsigned char) +
		NUMBER_OF_POINTERS * sizeof(PtrInt) + SIZE_OF_CHUNK_INFO_NEEDED * numberOfChunks;

	assert((uintptr_t)(pointer) >= (uintptr_t)(recentlyDeallocatedChunk) &&
		(uintptr_t)(pointer) < (uintptr_t)(recentlyDeallocatedChunk)+(uintptr_t)(PtrInt(blockSize) * (numberOfBlocks)));

	//assert(m_RecentlyDeallocatedChunk->m_PointerToData <= pointer);
	//assert(m_RecentlyDeallocatedChunk->m_PointerToData + (m_NumberOfBlocks * m_BlockSize) > pointer);

	const unsigned char recentlyDeallocatedChunkIndex =
		(firstChunkInAllocator - recentlyDeallocatedChunk) / (PtrInt(blockSize) * PtrInt(numberOfBlocks));

	ChunkDeallocation(rawRecentlyDeallocatedChunk, recentlyDeallocatedChunkIndex);
}

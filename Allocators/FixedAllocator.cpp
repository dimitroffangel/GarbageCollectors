#include "FixedAllocator.h"

#include <vector>
#include <assert.h>

#include "FastLogarithm.h"

FixedAllocator::FixedAllocator(std::size_t blockSize)
	:m_BlockSize(blockSize)
{
	assert(m_BlockSize > 0);

	m_PreviousFixedAlloc = m_NextFixedAlloc = this;

	std::size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;

	if (numBlocks > UNSIGNED_CHAR_MAX)
		numBlocks = UNSIGNED_CHAR_MAX;

	else if (numBlocks == 0)
	{
		numBlocks = DefaultNumberOfBlocks * blockSize;
	}

	m_NumberOfBlocks = static_cast<unsigned char>(numBlocks);
	assert(m_NumberOfBlocks == numBlocks);
}

FixedAllocator::~FixedAllocator()
{
	if (m_PreviousFixedAlloc != this)
	{
		m_PreviousFixedAlloc->m_NextFixedAlloc = m_NextFixedAlloc;
		m_NextFixedAlloc->m_PreviousFixedAlloc = m_PreviousFixedAlloc;
		return;
	}

	assert(m_PreviousFixedAlloc == m_NextFixedAlloc);

	Chunks::iterator it = m_Chunks.begin();

	for (; it != m_Chunks.end(); ++it)
	{
		assert(it->m_BlockAvailable == m_NumberOfBlocks);
		it->Release();
	}
}

FixedAllocator::FixedAllocator(const FixedAllocator& other)
{
	m_NumberOfBlocks = other.m_NumberOfBlocks;
	m_BlockSize = other.m_BlockSize;

	m_Chunks = std::vector<Chunk>(other.m_Chunks.begin(), other.m_Chunks.end());

	m_RecentlyAllocatedChunk = other.m_RecentlyAllocatedChunk;
	m_RecentlyDeallocatedChunk = other.m_RecentlyDeallocatedChunk;

	m_PreviousFixedAlloc = other.m_PreviousFixedAlloc;
	m_NextFixedAlloc = other.m_NextFixedAlloc;
}

FixedAllocator& FixedAllocator::operator=(const FixedAllocator& rhs)
{
	if (this != &rhs)
	{
		for (auto& chunk : m_Chunks)
		{
			chunk.Release();
		}

		m_NumberOfBlocks = rhs.m_NumberOfBlocks;

		m_Chunks = std::vector<Chunk>(rhs.m_Chunks.begin(), rhs.m_Chunks.end());

		//m_RecentlyAllocatedChunk = rhs.m_RecentlyAllocatedChunk;
		//m_RecentlyDeallocatedChunk = rhs.m_RecentlyDeallocatedChunk;

		//m_PreviousFixedAlloc = rhs.m_PreviousFixedAlloc;
		//m_NextFixedAlloc = rhs.m_NextFixedAlloc;
	}

	return *this;
}

void FixedAllocator::DoDeallocation(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedAllocator::DoDeallocation() pointer is nullptr" << '\n';
		return;
	}

	assert(m_RecentlyDeallocatedChunk != nullptr);
	assert((uintptr_t)(pointer) >= (uintptr_t)(m_RecentlyDeallocatedChunk->m_PointerToData) && 
		(uintptr_t)(pointer) < (uintptr_t)(m_RecentlyDeallocatedChunk->m_PointerToData) + (uintptr_t)(PtrInt(m_NumberOfBlocks) * PtrInt(m_BlockSize)));

	m_RecentlyDeallocatedChunk->Deallocate(pointer, m_BlockSize, m_NumberOfBlocks);

	if (m_RecentlyDeallocatedChunk->m_BlockAvailable == m_NumberOfBlocks)
	{
		Chunk& lastChunk = m_Chunks.back();

		if (&lastChunk == m_RecentlyDeallocatedChunk)
		{
			if ((m_Chunks.size() == 2 && m_Chunks[0].m_BlockAvailable == m_NumberOfBlocks) ||
				m_Chunks.size() > 1)
			{
				lastChunk.Release();
				m_Chunks.pop_back();

				// m_RecentlyDeallocatedChunk is pointing to something invalid
				m_RecentlyAllocatedChunk = m_RecentlyDeallocatedChunk = &m_Chunks.front();
			}

			return;
		}

		if (lastChunk.m_BlockAvailable == m_NumberOfBlocks)
		{
			lastChunk.Release();
			m_Chunks.pop_back();

			// should not be used
			m_RecentlyAllocatedChunk = m_RecentlyDeallocatedChunk = &m_Chunks.front();
		}

		else
		{
			// when swapping what happens with the address ??
			std::swap(*m_RecentlyDeallocatedChunk, m_Chunks.back());
			m_Chunks.back().Release();
			m_Chunks.pop_back();

			// should not be used
			m_RecentlyDeallocatedChunk = &m_Chunks.back();
		}
	}
}

Chunk* FixedAllocator::FindChunkWithPointer(void* pointerToFind)
{
	if (pointerToFind == nullptr)
	{
		std::cerr << "FixedAllocator::FindChunkWithPointer argument is nullptr " << '\n';
		return nullptr;
	}

	assert(!m_Chunks.empty());
	assert(m_RecentlyDeallocatedChunk != nullptr);

	bool assertHasFoundStorageChunk = false;

	for (size_t i = 0; i < m_Chunks.size(); ++i)
	{
		if ((uintptr_t)(pointerToFind) >= (uintptr_t)(m_Chunks[i].m_PointerToData) &&
			(uintptr_t)(pointerToFind) < (uintptr_t)(m_Chunks[i].m_PointerToData) + (uintptr_t)(PtrInt(m_BlockSize) * PtrInt(m_NumberOfBlocks)))
		{
			assertHasFoundStorageChunk = true;
			break;
		}
	}

	assert(assertHasFoundStorageChunk);

	//assert((uintptr_t)(pointerToFind) >= (uintptr_t)(&m_Chunks.front()) &&
	//	(uintptr_t)(pointerToFind) < (uintptr_t)(&m_Chunks.back())+(uintptr_t)(m_BlockSize * m_NumberOfBlocks));

	const std::size_t chunkLength = m_NumberOfBlocks * m_BlockSize;

	Chunk* low = m_RecentlyDeallocatedChunk;
	Chunk* high = m_RecentlyDeallocatedChunk + 1;
	Chunk* lowBound = &m_Chunks.front();
	Chunk* highBound = &m_Chunks.back() + 1;

	if (high == highBound)
		high = nullptr;

	for (;;)
	{
		if (low != nullptr)
		{
			if ((uintptr_t)(pointerToFind) >= (uintptr_t)(low->m_PointerToData) &&
				(uintptr_t)(pointerToFind) < (uintptr_t)(low->m_PointerToData) + (uintptr_t)+(chunkLength))
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
				--low;
			}
		}

		if (high != nullptr)
		{
			if ((uintptr_t)(pointerToFind) >= (uintptr_t)(high->m_PointerToData) &&
				(uintptr_t)(pointerToFind) < (uintptr_t)(high->m_PointerToData + chunkLength))
			{
				return high;
			}

			if (++high == highBound)
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

void* FixedAllocator::Allocate()
{
	if (m_RecentlyAllocatedChunk == nullptr ||
		m_RecentlyAllocatedChunk->m_BlockAvailable == 0)
	{
		Chunks::iterator it = m_Chunks.begin();

		for (;; ++it)
		{
			if (it == m_Chunks.end())
			{
				//m_Chunks.reserve(m_Chunks.size() + 1);
				Chunk newChunk;
				newChunk.Initialize(m_BlockSize, m_NumberOfBlocks);
				m_Chunks.push_back(newChunk);
				m_RecentlyAllocatedChunk = &m_Chunks.back();
				m_RecentlyDeallocatedChunk = &m_Chunks.front();
				break;
			}

			if (it->m_BlockAvailable > 0)
			{
				m_RecentlyAllocatedChunk = &(*it);
				break;
			}
		}
	}

	assert(m_RecentlyAllocatedChunk != nullptr);
	assert(m_RecentlyAllocatedChunk->m_BlockAvailable > 0);

	return m_RecentlyAllocatedChunk->Allocate(m_BlockSize);
}

void FixedAllocator::Deallocate(void* pointer)
{
	if (pointer == nullptr)
	{
		std::cerr << "FixedAllocator::Dealloacate() is nullptr" << '\n';
		return;
	}

	assert(!m_Chunks.empty());

	bool assertHasFoundStorageChunk = false;

	for (size_t i = 0; i < m_Chunks.size(); ++i)
	{
		if ((uintptr_t)(pointer) >= (uintptr_t)(m_Chunks[i].m_PointerToData) &&
			(uintptr_t)(pointer) < (uintptr_t)(m_Chunks[i].m_PointerToData) + (uintptr_t)(PtrInt(m_BlockSize) * PtrInt(m_NumberOfBlocks)))
		{
			assertHasFoundStorageChunk = true;
			break;
		}
	}


	assert(assertHasFoundStorageChunk);

	m_RecentlyDeallocatedChunk = FindChunkWithPointer(pointer);

	assert(m_RecentlyDeallocatedChunk != nullptr);

	DoDeallocation(pointer);
}
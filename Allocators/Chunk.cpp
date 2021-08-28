#include "Chunk.h"

#include <assert.h>

void Chunk::Initialize(std::size_t blockSize, unsigned char numberOfBlocks)
{
	m_PointerToData = new unsigned char[blockSize * numberOfBlocks];
	m_FirstAvailableBlock = 0;
	m_BlockAvailable = numberOfBlocks;
	unsigned char i = 0;
	unsigned char* tempPointer = m_PointerToData;

	for (; i != numberOfBlocks; tempPointer += blockSize)
	{
		*tempPointer = ++i;
	}
}

void Chunk::Release()
{
	delete[] m_PointerToData;
}

void Chunk::Reset(std::size_t blockSize, unsigned char numberOfBlocks)
{
	assert(blockSize > 0);
	assert(numberOfBlocks > 0);

	assert((blockSize * numberOfBlocks) / blockSize == numberOfBlocks);

	m_FirstAvailableBlock = 0;
	m_BlockAvailable = numberOfBlocks;

	unsigned char i = 0;

	unsigned char* pointer = m_PointerToData;

	for (; i != numberOfBlocks; pointer += blockSize)
	{
		*pointer = ++i;
	}
}

void* Chunk::Allocate(std::size_t blockSize)
{
	if (!m_BlockAvailable)
		return nullptr;

	unsigned char* pointerResult =
		m_PointerToData + (m_FirstAvailableBlock * blockSize);

	m_FirstAvailableBlock = *pointerResult;
	--m_BlockAvailable;

	return pointerResult;
}

void Chunk::Deallocate(void* deallocatePointer, size_t blockSize, const unsigned char numberOfBlocks)
{
	if(deallocatePointer == nullptr)
	{
		std::cerr << "Chunk::Deallocate void* deallocatePointer is nullptr" << '\n';
		return;
	}

	if (!((uintptr_t)(deallocatePointer) >= (uintptr_t)(m_PointerToData) &&
		(uintptr_t)(deallocatePointer) < (uintptr_t)(m_PointerToData)+ (uintptr_t)(blockSize * numberOfBlocks)))
	{
		std::cerr << "Chunk::Deallocate pointer is off the range of the Chunk" << '\n';
		return;
	}

	unsigned char* toReleasePointer = static_cast<unsigned char*>(deallocatePointer);

	// alignment check
	assert((toReleasePointer - m_PointerToData) % blockSize == 0);

	*toReleasePointer = m_FirstAvailableBlock;
	m_FirstAvailableBlock = static_cast<unsigned char>((toReleasePointer - m_PointerToData) / blockSize);

	// Truncation check
	assert(m_FirstAvailableBlock == (toReleasePointer - m_PointerToData) / blockSize);
	++m_BlockAvailable;
}

std::vector<unsigned char> Chunk::GetAvailableBlocks(const size_t blockSize) const
{
	if (m_BlockAvailable == 0)
	{
		return std::vector<unsigned char>();
	}

	std::vector<unsigned char> freeBlocks;
	freeBlocks.reserve(m_BlockAvailable);

	unsigned char currentBlockIndex = m_FirstAvailableBlock;
	while (true)
	{
		freeBlocks.push_back(currentBlockIndex);

		unsigned char* pointerResult =
			m_PointerToData + (m_FirstAvailableBlock * blockSize);


		currentBlockIndex = *pointerResult;

		if (freeBlocks.size() == m_BlockAvailable)
		{
			return freeBlocks;
		}
	}

	return freeBlocks;
}

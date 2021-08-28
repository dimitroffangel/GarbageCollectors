#ifndef CHUNK_H_GUARD
#define CHUNK_H_GUARD

#include <iostream>
#include <vector>

const size_t DEFAULT_CHUNK_SIZE = 4096;

const size_t UNSIGNED_CHAR_MAX = UCHAR_MAX;

const unsigned DefaultNumberOfBlocks = 8;

struct Chunk
{
	void Initialize(std::size_t blockSize, unsigned char numberOfBlocks);
	void Release();
	void Reset(std::size_t blockSize, unsigned char numberOfBlocks);
	void* Allocate(std::size_t blockSize);
	void Deallocate(void* pointer, const std::size_t blockSize, const unsigned char numberOfBlocks);
	unsigned char* m_PointerToData;
	unsigned char
		m_FirstAvailableBlock,
		m_BlockAvailable;
	
	
	std::vector<unsigned char> GetAvailableBlocks(const size_t blockSize) const;
};

#endif


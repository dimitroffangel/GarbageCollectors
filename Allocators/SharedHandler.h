#ifndef SHAREDHANDLER_H_GUARD
#define SHAREDHANDLER_H_GUARD

#include <assert.h>
#include <functional>
#include <vector>

#include "Chunk.h"

template <class T> 
class SharedHandler
{
public:

	SharedHandler(T* m_Pointer):
		m_Pointer(m_Pointer)
	{
		m_ReferenceCounter = new size_t(1);
	}

	SharedHandler(const SharedHandler& other):
		m_ReferenceCounter(other.m_ReferenceCounter),
		m_Pointer(other.m_Pointer)
	{
		++(*m_ReferenceCounter);
	}

	~SharedHandler()
	{
		if (*m_ReferenceCounter == 1)
		{
			m_AfterDeletionDelegate(m_Pointer);
			delete m_Pointer;
			delete m_ReferenceCounter;
			return;
		}

		--(*m_ReferenceCounter);
	}

	SharedHandler* operator=(const SharedHandler& rhs)
	{
		if (this != &rhs)
		{
			// if it is the last object remembering that object, annihilate it
			if (m_ReferenceCounter == 1)
			{
				m_AfterDeletionDelegate(m_Pointer);
				delete m_ReferenceCounter;
				delete m_Pointer;
			}

			assert(m_ReferenceCounter != 0);
			
			// m_ReferenceCounter >= 1
			--(*m_ReferenceCounter);

			m_ReferenceCounter = rhs.m_ReferenceCounter;
			m_Pointer = rhs.m_Pointer;
		}

		return *this;
	}

	operator bool() const
	{
		return m_Pointer != nullptr;
	}

	T* operator->() const
	{
		if (m_Pointer == nullptr)
		{
			std::cout << "SharedHandler::operator-> m_Pointer is nullptr" << '\n';
			return;
		}


		return m_Pointer;
	}

private:
	T* m_Pointer = nullptr;
	size_t* m_ReferenceCounter = nullptr;
	std::function<void<const Chunk&, void*, size_t>> m_AfterDeletionDelegate;
};

#endif


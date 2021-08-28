#ifndef UNITTESTS_H_GUARD
#define UNITTESTS_H_GUARD

#include "SmallObject.h"
#include "BuddyAllocator.h"
#include "BuddyAllocatorObject.h"
#include <random>

class Foo
{
public:
	int a = 42;
	int b = 32;

	int TryStuff(int c, int d, int f) const
	{
		return a + (c + d);
	}

	//~Foo()
	//{
	//	std::cout << "Losing Foo" << '\n';
	//}
};

class EpicFoo
{
	PtrInt first;
	PtrInt second;
	PtrInt third;
	PtrInt fourth;
	PtrInt fifth;
	PtrInt sixth;
	PtrInt seventh;
	PtrInt eighth;
	PtrInt eighth1;
	PtrInt eighth2;
	PtrInt eighth3;
	PtrInt eighth4;
	PtrInt eighth5;
	PtrInt eighth6;
	PtrInt eighth7;
	PtrInt eighth8;
};

class GiantFoo
{
	PtrInt coolInt_1;
	PtrInt coolInt_2;
	PtrInt coolInt_3;
	PtrInt coolInt_4; // 4 *sizeof(PtrInt) = 32  or 16

	Foo simpleFoo_1;
	Foo simpleFoo_2;
	Foo simpleFoo_3;
	Foo simpleFoo_4; // 4 * 8 = 32

	EpicFoo epicFoo_1;
	EpicFoo epicFoo_2;
	EpicFoo epicFoo_3;
	EpicFoo epicFoo_4; // 4 * 128 = 512
}; // 2^9 + 2^5 + 2^5 = 2^9 + 64 = 512 + 64 = 576 -> 64 bit machine
// 2^9 + 16 + 32 = 512 + 48 = 560 -> 32 bit machine  



enum TypeDelete
{
	BeginEnd,
	EndBegin,
	Random
};

struct PointerInformation
{
	void* pointer;
	size_t sizeOfObjectThere;
};

int GenerateRandomNumber(int from, int to);

struct UnitTests
{
	void Allocate_Via_Buddy_SmallObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Buddy_SmallObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Buddy_MediumObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Buddy_MediumObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Buddy_BigObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Buddy_BigObjects_Add_Delete(const BuddyAllocatorObject& , const SmallObject&);
	void Allocate_Via_Buddy_AllObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Buddy_RandomObject_Add_DeleteRandomPosition(const BuddyAllocatorObject& , const SmallObject&, const TypeDelete);

	void Allocate_Via_Slab_SmallObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Slab_SmallObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Slab_MediumObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Slab_MediumObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Slab_BigObjects(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Slab_BigObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Slab_AllObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject&);
	void Allocate_Via_Slab_RandomObject_DeleteRandomPosition(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);
	void Allocate_Via_Slab_RandomObject_Add_DeleteRandomPosition(const BuddyAllocatorObject&, const SmallObject&, const TypeDelete);

	void Allocate_Via_Default_SmallObjects(const TypeDelete);
	void Allocate_Via_Default_SmallObjects_Add_Delete();
	void Allocate_Via_Default_MediumObjects(const TypeDelete);
	void Allocate_Via_Default_MediumObjects_Add_Delete();
	void Allocate_Via_Default_BigObjects(const TypeDelete);
	void Allocate_Via_Default_BigObjects_Add_Delete();
	void Allocate_Via_Default_AllObjects_Add_Delete();
	void Allocate_Via_Default_RandomObject_DeleteRandomPosition(const TypeDelete);
	void Allocate_Via_Default_RandomObject_Add_DeleteRandomPosition(const TypeDelete);

	void Allocate_Via_MallocFree_SmallObjects(const TypeDelete);
	void Allocate_Via_MallocFree_SmallObjects_Add_Delete();
	void Allocate_Via_MallocFree_MediumObjects(const TypeDelete);
	void Allocate_Via_MallocFree_MediumObjects_Add_Delete();
	void Allocate_Via_MallocFree_BigObjects(const TypeDelete);
	void Allocate_Via_MallocFree_BigObjects_Add_Delete();
	void Allocate_Via_MallocFree_AllObjects_Add_Delete();
	void Allocate_Via_MallocFree_RandomObject_DeleteRandomPosition(const TypeDelete);
	void Allocate_Via_MallocFree_RandomObject_Add_DeleteRandomPosition(const TypeDelete);

};

#endif


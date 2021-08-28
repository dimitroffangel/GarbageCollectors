#include <iostream>
#include <vector>

#include "SmallObject.h"
#include "UnitTests.h"

int main()
{	
	SmallObjectAllocator::Initialize();
	BuddyAllocator::Initialize();

	SmallObject smallObject;
	BuddyAllocatorObject buddyAllocator;

	UnitTests unitTests;

	std::cout << "Buddy allocator..." << '\n';

	unitTests.Allocate_Via_Buddy_BigObjects(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Buddy_BigObjects(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Slab_BigObjects(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Slab_BigObjects(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Default_BigObjects(TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Default_BigObjects(TypeDelete::EndBegin);

	std::cout << '\n';
	unitTests.Allocate_Via_Buddy_BigObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_BigObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Default_BigObjects_Add_Delete();

	std::cout << '\n';
	unitTests.Allocate_Via_Buddy_AllObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_AllObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Default_AllObjects_Add_Delete();

	std::cout << '\n';
	unitTests.Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Slab_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Slab_RandomObject_DeleteRandomPosition(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Default_RandomObject_DeleteRandomPosition(TypeDelete::EndBegin);

	std::cout << '\n';
	unitTests.Allocate_Via_Buddy_SmallObjects(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Buddy_SmallObjects(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Slab_SmallObjects(buddyAllocator, smallObject, TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Slab_SmallObjects(buddyAllocator, smallObject, TypeDelete::EndBegin);
	unitTests.Allocate_Via_Default_SmallObjects(TypeDelete::BeginEnd);
	unitTests.Allocate_Via_Default_SmallObjects(TypeDelete::EndBegin);

	std::cout << '\n';
	unitTests.Allocate_Via_Buddy_SmallObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Slab_SmallObjects_Add_Delete(buddyAllocator, smallObject);
	unitTests.Allocate_Via_Default_SmallObjects_Add_Delete();


	//std::cout << "Slab allocator..." << '\n';


	//std::cout << "Default allocator..." << '\n';

	//unitTests.Allocate_Via_Default_BigObjects(TypeDelete::Random);

	SmallObjectAllocator::Shutdown();
	BuddyAllocator::Shutdown();

	return 0;
}
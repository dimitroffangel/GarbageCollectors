#include "UnitTests.h"

#include <chrono>

#include "SmallObjectAllocator.h"
#include <bitset>

int GenerateRandomNumber(int from, int to)
{
	if (from > to)
	{
		std::cout << "UnitTest::GenerateRandomNumber() from > to" << '\n';
		return 0;
	}

	static std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
	static std::mt19937 generatorOfRandomNumbers(randomDevice()); //Standard mersenne_twister_engine seeded with rd()

	std::uniform_int_distribution<> generatedRandomNumbers(from, to);

	return generatedRandomNumbers(generatorOfRandomNumbers);
}

void UnitTests::Allocate_Via_Buddy_SmallObjects(const BuddyAllocatorObject& buddyAllocator, 
	const SmallObject& smallObject, const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_SmallObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~Foo();

			buddyAllocator.operator delete(foos[size - i - 1], sizeof(Foo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~Foo();

			buddyAllocator.operator delete(foos[i], sizeof(Foo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			foos[randomNumber]->~Foo();

			buddyAllocator.operator delete(foos[randomNumber], sizeof(Foo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_SmallObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject&)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_SmallObjects_Add_Delete(): ";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();

		buddyAllocator.operator delete(foos[i], sizeof(Foo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_MediumObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject,
	const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_MediumObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		EpicFoo* result = new (rawPointer) EpicFoo();

		foos.push_back(result);
	}


	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~EpicFoo();

			buddyAllocator.operator delete(foos[size - i - 1], sizeof(EpicFoo));
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~EpicFoo();

			buddyAllocator.operator delete(foos[i], sizeof(EpicFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			foos[randomNumber]->~EpicFoo();

			buddyAllocator.operator delete(foos[randomNumber], sizeof(EpicFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_MediumObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObjects)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_MediumObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();

		foos.push_back(result);

		foos[i]->~EpicFoo();
		buddyAllocator.operator delete(foos[i], sizeof(GiantFoo));
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';

}

void UnitTests::Allocate_Via_Buddy_BigObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject,
	const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_BigObjects(): ";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~GiantFoo();

			buddyAllocator.operator delete(foos[size - i - 1], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~GiantFoo();

			buddyAllocator.operator delete(foos[i], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		std::bitset<size> hasElementBeenDeleted;
		hasElementBeenDeleted.reset();

		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			while (hasElementBeenDeleted[randomNumber])
			{
				randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);
			}

			hasElementBeenDeleted[randomNumber] = 1;

			foos[randomNumber]->~GiantFoo();

			buddyAllocator.operator delete(foos[randomNumber], sizeof(GiantFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_BigObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_BigObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);

		foos[i]->~GiantFoo();
		buddyAllocator.operator delete(foos[i], sizeof(GiantFoo));
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_AllObjects_Add_Delete(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_AllObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();
		buddyAllocator.operator delete(foos[i], sizeof(Foo));

		void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		giantFoos[i]->~GiantFoo();
		buddyAllocator.operator delete(giantFoos[i], sizeof(GiantFoo));


		void* rawPointer3 = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer2) EpicFoo();

		epicFoos.push_back(res3);

		epicFoos[i]->~EpicFoo();
		buddyAllocator.operator delete(epicFoos[i], sizeof(EpicFoo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject,
	const TypeDelete typeDelete)
{	
	std::cout << "UnitTests::Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();

		epicFoos.push_back(result);
		
		void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		void* rawPointer3 = buddyAllocator.operator new(sizeof(Foo));

		Foo* res3 = new (rawPointer3) Foo();

		foos.push_back(res3);
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[size - i - 1]->~Foo();
			buddyAllocator.operator delete(foos[size - i - 1], sizeof(Foo));

			epicFoos[size - i - 1]->~EpicFoo();
			buddyAllocator.operator delete(epicFoos[size - i - 1], sizeof(EpicFoo));

			giantFoos[size - i - 1]->~GiantFoo();
			buddyAllocator.operator delete(giantFoos[size - i - 1], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[i]->~Foo();
			buddyAllocator.operator delete(foos[i], sizeof(Foo));

			epicFoos[i]->~EpicFoo();
			buddyAllocator.operator delete(epicFoos[i], sizeof(EpicFoo));

			giantFoos[i]->~GiantFoo();
			buddyAllocator.operator delete(giantFoos[i], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumberFoo = GenerateRandomNumber(0, foos.size() - i - 1);
			int randomNumberEpicFoo = GenerateRandomNumber(0, epicFoos.size() - i - 1);
			int randomNumberGiantFoo = GenerateRandomNumber(0, giantFoos.size() - i - 1);

			foos[randomNumberFoo]->~Foo();
			buddyAllocator.operator delete(foos[i], sizeof(Foo));

			epicFoos[randomNumberEpicFoo]->~EpicFoo();
			buddyAllocator.operator delete(epicFoos[i], sizeof(EpicFoo));

			giantFoos[randomNumberGiantFoo]->~GiantFoo();
			buddyAllocator.operator delete(giantFoos[i], sizeof(GiantFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Buddy_RandomObject_Add_DeleteRandomPosition(const BuddyAllocatorObject& buddyAllocator, 
	const SmallObject&, const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Buddy_RandomObject_DeleteRandomPosition(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<PointerInformation> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		const int randomNumber = GenerateRandomNumber(0, 2);

		if (randomNumber == 0)
		{
			void* rawPointer = buddyAllocator.operator new(sizeof(EpicFoo));

			EpicFoo* result = new (rawPointer) EpicFoo();

			foos.push_back({ rawPointer, sizeof(EpicFoo) });
		}

		
		if (randomNumber == 1)
		{
			void* rawPointer2 = buddyAllocator.operator new(sizeof(GiantFoo));

			GiantFoo* res2 = new (rawPointer2) GiantFoo();

			foos.push_back({ rawPointer2, sizeof(GiantFoo) });
		}

		if (randomNumber == 2)
		{
			void* rawPointer3 = buddyAllocator.operator new(sizeof(Foo));

			Foo* res3 = new (rawPointer3) Foo();

			foos.push_back({ rawPointer3, sizeof(Foo) });
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = size - i - 1;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = i;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				buddyAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_SmallObjects(const BuddyAllocatorObject& buddyAllocator, const SmallObject& smallObject, 
	const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_SmallObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = smallObject.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~Foo();

			smallObject.operator delete(foos[size - i - 1], sizeof(Foo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~Foo();

			smallObject.operator delete(foos[i], sizeof(Foo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, size - i - 1);
			
			foos[randomNumber]->~Foo();

			smallObject.operator delete(foos[randomNumber], sizeof(Foo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_MediumObjects(const BuddyAllocatorObject& buddyObject, const SmallObject& smallObject, 
	const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_MediumObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();

		foos.push_back(result);
	}


	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~EpicFoo();

			smallObject.operator delete(foos[size - i - 1], sizeof(EpicFoo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~EpicFoo();

			smallObject.operator delete(foos[i], sizeof(EpicFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, size - i - 1);
			foos[randomNumber]->~EpicFoo();

			smallObject.operator delete(foos[randomNumber], sizeof(EpicFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_MediumObjects_Add_Delete(const BuddyAllocatorObject& buddyObject, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Slab_MediumObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeof(EpicFoo));

		EpicFoo* result = new (rawPointer) EpicFoo();

		foos.push_back(result);

		foos[i]->~EpicFoo();

		smallObject.operator delete(foos[i], sizeof(EpicFoo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_SmallObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Slab_SmallObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		void* rawPointer = smallObject.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();

		smallObject.operator delete(foos[i], sizeof(Foo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_BigObjects(const BuddyAllocatorObject&, const SmallObject& smallObject, const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_BigObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);
	}


	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~GiantFoo();

			smallObject.operator delete(foos[size - i - 1], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~GiantFoo();

			smallObject.operator delete(foos[i], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, size - i - 1);
			foos[randomNumber]->~GiantFoo();

			smallObject.operator delete(foos[randomNumber], sizeof(GiantFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';

}

void UnitTests::Allocate_Via_Slab_BigObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Slab_BigObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeof(GiantFoo));

		GiantFoo* result = new (rawPointer) GiantFoo();

		foos.push_back(result);

		foos[i]->~GiantFoo();
		smallObject.operator delete(foos[i], sizeof(GiantFoo));
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_RandomObject_DeleteRandomPosition(const BuddyAllocatorObject&, const SmallObject& smallObject, 
	const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_RandomObject_DeleteRandomPosition(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeof(Foo));

		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);


		void* rawPointer2 = smallObject.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		void* rawPointer3 = smallObject.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer3) EpicFoo();

		epicFoos.push_back(res3);
	}


	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[size - i - 1]->~Foo();
			smallObject.operator delete(foos[size - i - 1], sizeof(Foo));

			epicFoos[size - i - 1]->~EpicFoo();
			smallObject.operator delete(epicFoos[size - i - 1], sizeof(EpicFoo));

			giantFoos[size - i - 1]->~GiantFoo();
			smallObject.operator delete(giantFoos[size - i - 1], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[i]->~Foo();
			smallObject.operator delete(foos[i], sizeof(Foo));

			epicFoos[i]->~EpicFoo();
			smallObject.operator delete(epicFoos[i], sizeof(EpicFoo));

			giantFoos[i]->~GiantFoo();
			smallObject.operator delete(giantFoos[i], sizeof(GiantFoo));
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = GenerateRandomNumber(0, size - i - 1);

			foos[randomNumber]->~Foo();
			smallObject.operator delete(foos[randomNumber], sizeof(Foo));

			epicFoos[randomNumber]->~EpicFoo();
			smallObject.operator delete(epicFoos[randomNumber], sizeof(EpicFoo));

			giantFoos[randomNumber]->~GiantFoo();
			smallObject.operator delete(giantFoos[randomNumber], sizeof(GiantFoo));
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';

}

void UnitTests::Allocate_Via_Slab_RandomObject_Add_DeleteRandomPosition(const BuddyAllocatorObject&,
	const SmallObject& slabAllocator, const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_RandomObject_Add_DeleteRandomPosition() ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<PointerInformation> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		const int randomNumber = GenerateRandomNumber(0, 2);

		if (randomNumber == 0)
		{
			void* rawPointer = slabAllocator.operator new(sizeof(EpicFoo));

			EpicFoo* result = new (rawPointer) EpicFoo();

			foos.push_back({ rawPointer, sizeof(EpicFoo) });
		}


		if (randomNumber == 1)
		{
			void* rawPointer2 = slabAllocator.operator new(sizeof(GiantFoo));

			GiantFoo* res2 = new (rawPointer2) GiantFoo();

			foos.push_back({ rawPointer2, sizeof(GiantFoo) });
		}

		if (randomNumber == 2)
		{
			void* rawPointer3 = slabAllocator.operator new(sizeof(Foo));

			Foo* res3 = new (rawPointer3) Foo();

			foos.push_back({ rawPointer3, sizeof(Foo) });
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = size - i - 1;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = i;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				static_cast<Foo*>(foos[randomNumber].pointer)->~Foo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(Foo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				static_cast<EpicFoo*>(foos[randomNumber].pointer)->~EpicFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(EpicFoo));
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				static_cast<GiantFoo*>(foos[randomNumber].pointer)->~GiantFoo();
				slabAllocator.operator delete(foos[randomNumber].pointer, sizeof(GiantFoo));
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Slab_AllObjects_Add_Delete(const BuddyAllocatorObject&, const SmallObject& smallObject)
{
	std::cout << "UnitTests::Allocate_Via_Slab_AllObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = smallObject.operator new(sizeOfFoo);
		
		Foo* result = new (rawPointer) Foo();

		foos.push_back(result);

		foos[i]->~Foo();
		smallObject.operator delete(foos[i], sizeof(Foo));

		void* rawPointer2 = smallObject.operator new(sizeof(GiantFoo));

		GiantFoo* res2 = new (rawPointer2) GiantFoo();

		giantFoos.push_back(res2);

		giantFoos[i]->~GiantFoo();
		smallObject.operator delete(giantFoos[i], sizeof(GiantFoo));

		void* rawPointer3 = smallObject.operator new(sizeof(EpicFoo));

		EpicFoo* res3 = new (rawPointer3) EpicFoo();

		epicFoos.push_back(res3);

		epicFoos[i]->~EpicFoo();

		smallObject.operator delete(epicFoos[i], sizeof(EpicFoo));
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_SmallObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Default_SmallObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		Foo* result1 = new Foo();
		foos.push_back(result1);
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[size - i - 1];
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[i];
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			delete foos[size - i - 1];
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_SmallObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_Default_SmallObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		Foo* result = new Foo();

		foos.push_back(result);

		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_MediumObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests:: UnitTests::Allocate_Via_Default_MediumObjects()";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		EpicFoo* result1 = new EpicFoo();
		foos.push_back(result1);
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[i];
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[size - i - 1];
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			int randomNumber = GenerateRandomNumber(0, size - i - 1);

			delete foos[randomNumber];
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_MediumObjects_Add_Delete()
{
	std::cout << "UnitTests:: Allocate_Via_Default_MediumObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{ 
		EpicFoo* result = new EpicFoo();

		foos.push_back(result);

		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_AllObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_Default_AllObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		//void* rawPointer = smallObject.operator new(sizeOfFoo);
		
		Foo* result = new Foo();

		foos.push_back(result);

		delete foos[i];
		
		GiantFoo* res2 = new GiantFoo();

		giantFoos.push_back(res2);

		delete giantFoos[i];


		EpicFoo* res3 = new EpicFoo();

		epicFoos.push_back(res3);

		delete epicFoos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_BigObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Default_BigObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		GiantFoo* result1 = new GiantFoo();
		foos.push_back(result1);
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[i];
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete foos[size - i - 1];
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);
			delete foos[randomNumber];
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_BigObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_Default_BigObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		GiantFoo* result1 = new GiantFoo();
		foos.push_back(result1);
	
		delete foos[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_RandomObject_DeleteRandomPosition(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Default_RandomObject_DeleteRandomPosition(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		Foo* result = new Foo();

		foos.push_back(result);

		GiantFoo* res2 = new GiantFoo();

		giantFoos.push_back(res2);

		EpicFoo* res3 = new EpicFoo();

		epicFoos.push_back(res3);
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			delete foos[i];

			delete epicFoos[i];

			delete giantFoos[i];
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			delete foos[size - i - 1];

			delete epicFoos[size - i - 1];

			delete giantFoos[size - i - 1];
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; ++i)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			delete foos[randomNumber];

			delete epicFoos[randomNumber];

			delete giantFoos[randomNumber];
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_Default_RandomObject_Add_DeleteRandomPosition(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_Slab_RandomObject_Add_DeleteRandomPosition() ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<PointerInformation> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		const int randomNumber = GenerateRandomNumber(0, 2);

		if (randomNumber == 0)
		{
			EpicFoo* result = new EpicFoo();

			foos.push_back({ result, sizeof(EpicFoo) });
		}


		if (randomNumber == 1)
		{
			GiantFoo* res2 = new GiantFoo();

			foos.push_back({ res2, sizeof(GiantFoo) });
		}

		if (randomNumber == 2)
		{
			Foo* res3 = new Foo();

			foos.push_back({ res3, sizeof(Foo) });
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = size - i - 1;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				delete currentObjectToDelete;
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = i;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				delete currentObjectToDelete;
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			const int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				delete currentObjectToDelete;
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				delete currentObjectToDelete;
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_SmallObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests:: Allocate_Via_MallocFree_SmallObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(Foo));
		Foo* result1 = new (rawPointer) (Foo);
		foos.push_back(result1);
	}


	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~Foo();
			free(foos[i]);
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~Foo();
			free(foos[size - i - 1]);
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			foos[randomNumber]->~Foo();
			free(foos[randomNumber]);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_SmallObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_SmallObjects_Add_Delete";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(Foo));
		Foo* result1 = new (rawPointer) (Foo);
		foos.push_back(result1);

		foos[i]->~Foo();
		free(foos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_MediumObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_MediumObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(EpicFoo));
		EpicFoo* result1 = new (rawPointer) (EpicFoo);
		foos.push_back(result1);
	}

	
	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~EpicFoo();
			free(foos[i]);
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~EpicFoo();
			free(foos[size - i - 1]);
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			foos[randomNumber]->~EpicFoo();
			free(foos[randomNumber]);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_MediumObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_MediumObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<EpicFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(EpicFoo));
		EpicFoo* result1 = new (rawPointer) (EpicFoo);
		foos.push_back(result1);

		foos[i]->~EpicFoo();
		free(foos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_BigObjects(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_BigObjects(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(GiantFoo));
		GiantFoo* result1 = new (rawPointer) (GiantFoo);
		foos.push_back(result1);
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[i]->~GiantFoo();
			free(foos[i]);
		}
	}
	
	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; i++)
		{
			foos[size - i - 1]->~GiantFoo();
			free(foos[size - i - 1]);
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; i++)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			foos[randomNumber]->~GiantFoo();
			free(foos[randomNumber]);
		}
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_BigObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_BigObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<GiantFoo*> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(GiantFoo));
		GiantFoo* result1 = new (rawPointer) (GiantFoo);
		foos.push_back(result1);

		foos[i]->~GiantFoo();
		free(foos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';

}

void UnitTests::Allocate_Via_MallocFree_AllObjects_Add_Delete()
{
	std::cout << "UnitTests::Allocate_Via_Default_AllObjects_Add_Delete(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(Foo));
		Foo* result1 = new (rawPointer) (Foo);
		foos.push_back(result1);

		foos[i]->~Foo();
		free(foos[i]);

		void* rawPointer2 = malloc(sizeof(GiantFoo));
		GiantFoo* result2 = new (rawPointer2) (GiantFoo);
		giantFoos.push_back(result2);

		giantFoos[i]->~GiantFoo();
		free(giantFoos[i]);

		void* rawPointer3 = malloc(sizeof(EpicFoo));
		EpicFoo* result3 = new (rawPointer3) (EpicFoo);
		epicFoos.push_back(result3);

		epicFoos[i]->~EpicFoo();
		free(epicFoos[i]);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

void UnitTests::Allocate_Via_MallocFree_RandomObject_DeleteRandomPosition(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_RandomObject_DeleteRandomPosition(): ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<Foo*> foos;
	foos.reserve(size);

	std::vector<GiantFoo*> giantFoos;
	giantFoos.reserve(size);

	std::vector<EpicFoo*> epicFoos;
	epicFoos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		void* rawPointer = malloc(sizeof(Foo));
		Foo* result1 = new (rawPointer) (Foo);
		foos.push_back(result1);

		void* rawPointer2 = malloc(sizeof(EpicFoo));
		EpicFoo* result2 = new (rawPointer2) (EpicFoo);
		epicFoos.push_back(result2);

		void* rawPointer3 = malloc(sizeof(EpicFoo));
		EpicFoo* result3 = new (rawPointer3) (EpicFoo);
		epicFoos.push_back(result3);
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[i]->~Foo();
			free(foos[i]);

			epicFoos[i]->~EpicFoo();
			free(epicFoos[i]);

			giantFoos[i]->~GiantFoo();
			free(giantFoos[i]);
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			foos[size - i - 1]->~Foo();
			free(foos[size - i - 1]);

			epicFoos[size - i - 1]->~EpicFoo();
			free(epicFoos[size - i - 1]);

			giantFoos[size - i - 1]->~GiantFoo();
			free(giantFoos[size - i - 1]);
		}
	}

	if (typeDelete == TypeDelete::Random)
	{
		for (size_t i = 0; i < size; ++i)
		{
			const int randomNumber = GenerateRandomNumber(0, size - i - 1);

			foos[randomNumber]->~Foo();
			free(foos[randomNumber]);

			epicFoos[randomNumber]->~EpicFoo();
			free(epicFoos[i]);

			giantFoos[randomNumber]->~GiantFoo();
			free(giantFoos[randomNumber]);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';

}

void UnitTests::Allocate_Via_MallocFree_RandomObject_Add_DeleteRandomPosition(const TypeDelete typeDelete)
{
	std::cout << "UnitTests::Allocate_Via_MallocFree_RandomObject_Add_DeleteRandomPosition() ";

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	const int blocks = 4;
	const size_t sizeOfPtrInt = sizeof(PtrInt);
	const size_t sizeOfFoo = sizeof(Foo);
	const size_t sizeOfEpicFoo = sizeof(EpicFoo);
	const size_t sizeofGiantFoo = sizeof(GiantFoo);

	const int size = 10000;

	std::vector<PointerInformation> foos;
	foos.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		const int randomNumber = GenerateRandomNumber(0, 2);

		if (randomNumber == 0)
		{
			void* rawPointer = malloc(sizeof(EpicFoo));
			EpicFoo* result1 = new (rawPointer) (EpicFoo);
			
			foos.push_back({ result1, sizeof(EpicFoo) });
		}


		if (randomNumber == 1)
		{
			void* rawPointer = malloc(sizeof(GiantFoo));
			GiantFoo* result1 = new (rawPointer) (GiantFoo);

			foos.push_back({ result1, sizeof(GiantFoo) });
		}

		if (randomNumber == 2)
		{
			void* rawPointer = malloc(sizeof(Foo));
			Foo* result1 = new (rawPointer) (Foo);

			foos.push_back({ result1, sizeof(Foo) });
		}
	}

	if (typeDelete == TypeDelete::EndBegin)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = size - i - 1;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				free(currentObjectToDelete);
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			int randomNumber = i;

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				free(currentObjectToDelete);
			}
		}
	}

	if (typeDelete == TypeDelete::BeginEnd)
	{
		for (size_t i = 0; i < size; ++i)
		{
			const int randomNumber = GenerateRandomNumber(0, foos.size() - i - 1);

			if (foos[randomNumber].sizeOfObjectThere == sizeof(Foo))
			{
				Foo* currentObjectToDelete = static_cast<Foo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~Foo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(EpicFoo))
			{
				EpicFoo* currentObjectToDelete = static_cast<EpicFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~EpicFoo();
				free(currentObjectToDelete);
			}

			else if (foos[randomNumber].sizeOfObjectThere == sizeof(GiantFoo))
			{
				GiantFoo* currentObjectToDelete = static_cast<GiantFoo*>(foos[randomNumber].pointer);
				currentObjectToDelete->~GiantFoo();
				free(currentObjectToDelete);
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << '\n';
}

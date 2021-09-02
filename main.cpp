#include <iostream>

#include "MarkSweepGC.h"

int main()
{
	//std::cout << _CXX17;

	int* a = new int(42);

	int** b = &a;

	*b = new int(21);

	std::cout << *a;

	CreateGarbageCollector(0, nullptr);

	return 0;
}
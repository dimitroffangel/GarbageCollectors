#include <iostream>

int main()
{
	int* a = new int(42);

	int** b = &a;

	*b = new int(21);

	std::cout << *a;

	return 0;
}
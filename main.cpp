#include "vec.h"
#include <iostream>

int main()
{
	Vec<int> t(10);

	t.push_back(11);

	std::cout << t[0] << std::endl;

	t.emplace(t.begin() + 12, 22);
	
	return 0;
}
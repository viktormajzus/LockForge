#include <iostream>
#include <botan/version.h>

int main()
{
	std::cout << Botan::version_string() << "\n";
	return 0;
}

#include "System.h"

int main()
{
	System system;

	system.begin();
	system.run();
	system.shutdown();

	return;
}
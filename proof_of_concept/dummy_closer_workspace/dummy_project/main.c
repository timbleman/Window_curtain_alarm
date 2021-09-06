#include <stdio.h>
#include "configuration.h"

#ifndef UNITTESTS_INSTEAD_OF_MAIN
int main(int argc, char **argv)
#else
int main_nottest(void)
#endif // UNITTESTS_INSTEAD_OF_MAIN
{
	printf("hello world\n");
	return 0;
}

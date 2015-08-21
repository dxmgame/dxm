#include "precompiled.h"
#include <stdio.h>

#include <map>

#define XE_DOT ,
int main(int argc, char **argv)
{
	std::map<int XE_DOT int> map_test;
	map_test[1] = 10;
	printf("hello world %d\n", map_test[1]);
}

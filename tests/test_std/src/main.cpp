#include "precompiled.h"
#include <stdio.h>
#include <vector>

int main(int argc, char **argv)
{
	// ²âÊÔvectorµÄÉ¾³ı;
	std::vector<int> num_array;
	num_array.resize(10);

	for (int i = 0; i < num_array.size(); ++i){
		printf("%d:%d\n", i, num_array[i]);
	}

	for (int i = 0; i < num_array.size(); ++i){
		num_array[i] = i;
		printf("%d:%d\n", i, num_array[i]);
	}

	num_array.erase(num_array.begin()+5);

	for (int i = 0; i < num_array.size(); ++i){
		printf("%d:%d\n", i, num_array[i]);
	}

	printf("hello world\n");
}

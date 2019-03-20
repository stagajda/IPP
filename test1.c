#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main(){
	char c[4] = {0,1,2,3};
	int a = atoi(c);
	printf("%d\n", a);
	/*
	uint64_t x = UINTMAX_MAX;
	printf("%" PRIu64 "\n",x );
	*/
}

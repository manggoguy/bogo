#include <stdio.h>
#include <immintrin.h>

int main(void)
{
    int* ptr;
    int a = 3;
	unsigned int i=_xbegin();
	if( i==_XBEGIN_STARTED){
    	ptr = &a;
		_xend();
	}
	else
		printf("TX Failed\n");
    printf("%d\n", *ptr);
    
    return 0;
}


/*
 * Unit test program for Lib LBR
 * Jun 2015 Tong Zhang <ztong@vt.edu>
 */

#include <lbr.h>

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>


#define _USE_CPU_AFFINITY_


#ifdef _USE_CPU_AFFINITY_
void set_cpu_affinity()
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0,&mask);//use cpu 0
	int result = sched_setaffinity(0, sizeof(mask), &mask);
	if(result)
	{
		fprintf(stderr,"set cpu affinity failed, use taskset instead\n");
	}
}
#endif


/*
 * test functions
 */
void func7()
{
	/*
	 * dump data from LBR
	 */
	lbr_stack lbrstack;
	dump_lbr(0,&lbrstack);
	inteprete_lbr_info(&lbrstack);
}

void func6()
{
	func7();
}
void func5()
{
	func6();
}
void func4()
{
	func5();
}
void func3()
{
	func4();
}
void func2()
{
	func3();
}
void func1()
{
	func2();
}
void func0()
{
	func1();
}

int main()
{
#ifdef _USE_CPU_AFFINITY_
	set_cpu_affinity();
#endif
	if(start_lbr(0))
	{
		return -1;
	}

	func0();
	cleanup_lbr(0);
	return 0;
}



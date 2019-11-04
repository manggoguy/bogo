/*
 * LBR read test
 *
 * 2015 Tong Zhang <ztong@vt.edu>
 *
 * tested on Intel i7-4790(Haswell), Linux-3.19.4-gentoo (64bit)
 * need msr kernel module
 *
 * usage: gcc -g readlbr.c -o readlbr
 *         taskset -c 0 ./readlbr
 *         addr2line -a [address] -e ./readlbr -f
 *
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>

/*
 * set program to run on cpu 0 instead of using taskset
 */

#define _USE_CPU_AFFINITY_


/*
 * Reference: Intel Arch Software Developer's Manual Vol 3 B 17.8
 */

/*
 * MSR should be accessed from ring0
 * use kernel facility to access msr
 */

#define MSR_DEV_FILE "/dev/cpu/0/msr"

/*
 * Register Offsets
 */

#define MSR_LBR_SELECT (0x1C8)

#define MSR_LASTBRANCH_TOS (0x1C9)

#define IA32_DEBUGCTL (0x1D9)

#define MSR_LASTBRANCH_0_FROM_IP (0x680)
#define MSR_LASTBRANCH_1_FROM_IP (0x681)
#define MSR_LASTBRANCH_2_FROM_IP (0x682)
#define MSR_LASTBRANCH_3_FROM_IP (0x683)
#define MSR_LASTBRANCH_4_FROM_IP (0x684)
#define MSR_LASTBRANCH_5_FROM_IP (0x685)
#define MSR_LASTBRANCH_6_FROM_IP (0x686)
#define MSR_LASTBRANCH_7_FROM_IP (0x687)
#define MSR_LASTBRANCH_8_FROM_IP (0x688)
#define MSR_LASTBRANCH_9_FROM_IP (0x689)
#define MSR_LASTBRANCH_10_FROM_IP (0x68A)
#define MSR_LASTBRANCH_11_FROM_IP (0x68B)
#define MSR_LASTBRANCH_12_FROM_IP (0x68C)
#define MSR_LASTBRANCH_13_FROM_IP (0x68D)
#define MSR_LASTBRANCH_14_FROM_IP (0x68E)
#define MSR_LASTBRANCH_15_FROM_IP (0x68F)

#define MSR_LASTBRANCH_0_TO_IP (0x6C0)
#define MSR_LASTBRANCH_1_TO_IP (0x6C1)
#define MSR_LASTBRANCH_2_TO_IP (0x6C2)
#define MSR_LASTBRANCH_3_TO_IP (0x6C3)
#define MSR_LASTBRANCH_4_TO_IP (0x6C4)
#define MSR_LASTBRANCH_5_TO_IP (0x6C5)
#define MSR_LASTBRANCH_6_TO_IP (0x6C6)
#define MSR_LASTBRANCH_7_TO_IP (0x6C7)
#define MSR_LASTBRANCH_8_TO_IP (0x6C8)
#define MSR_LASTBRANCH_9_TO_IP (0x6C9)
#define MSR_LASTBRANCH_10_TO_IP (0x6CA)
#define MSR_LASTBRANCH_11_TO_IP (0x6CB)
#define MSR_LASTBRANCH_12_TO_IP (0x6CC)
#define MSR_LASTBRANCH_13_TO_IP (0x6CD)
#define MSR_LASTBRANCH_14_TO_IP (0x6CE)
#define MSR_LASTBRANCH_15_TO_IP (0x6CF)

/*
 * Refer 17.4.8.1 LBR Stack and Intel 64 Processors
 */
#define IA32_DEBUGCTL_LBR_MASK (0x00000001)
#define IA32_DEBUGCTL_FRZ_LBRS_ON_PMI_MASK (0x00000080)

#define MSR_LBR_SELECT_CPL_EQ_0_MASK (0x00000001)

int msr_fd;

typedef struct _lbr_stack
{
	unsigned long msr_lastbranch_from_ip[16];
	unsigned long msr_lastbranch_to_ip[16];
} lbr_stack;

lbr_stack lbrstack;


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

int init_lbr()
{
	msr_fd = open(MSR_DEV_FILE,O_RDWR);
	if(msr_fd==-1)
	{
		fprintf(stderr,"can not open msr file %s\n",MSR_DEV_FILE);
		return -1;
	}
	/*
	 * Step 1
	 * IA32_DEBUGCTL MSR (Figure 17-11)
	 * set LBR, bit 0
	 * set FREEZE_LBRS_ON_PMI, bit 11
	 */
	unsigned long ia32_debugctl;
	pread(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL);
	ia32_debugctl |= IA32_DEBUGCTL_LBR_MASK;
	ia32_debugctl |= IA32_DEBUGCTL_FRZ_LBRS_ON_PMI_MASK;
	pwrite(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL);

	/*
	 * Step 2
	 * TOS MSR that points to the last valid from/to pair.
	 * according to manual, the TOS register is RO?
	 */

	/*
	 * Step 3
	 * MSR Filter
	 * MSR_LBR_SELECT for Haswell
	 * set CPL_EQ_0, bit 0,  do not capture branches in ring 0
	 * set EN_CALLSTACK?, bit 9, Enable LBR stack to use LIFO filtering to capture Call stack profile?
	 * Reference: 17.8 Table 17-12
	 */
	unsigned long msr_lbr_select;
	pread(msr_fd,&msr_lbr_select,sizeof(unsigned long),MSR_LBR_SELECT);
	msr_lbr_select |= MSR_LBR_SELECT_CPL_EQ_0_MASK;
	pwrite(msr_fd,&msr_lbr_select,sizeof(unsigned long),MSR_LBR_SELECT);

	return 0;
}

#define stop_lbr \
	{ \
		unsigned long ia32_debugctl; \
		pread(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL); \
		ia32_debugctl &= ~IA32_DEBUGCTL_LBR_MASK; \
		ia32_debugctl &= ~IA32_DEBUGCTL_FRZ_LBRS_ON_PMI_MASK; \
		pwrite(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL); \
	}

void cleanup_lbr()
{
	/*
	 * reset LBR
	 */

	stop_lbr;

	close(msr_fd);
}

#define read_lbr_stack_pair(X) \
	rsum+= pread(msr_fd,&lbrstack.msr_lastbranch_from_ip[X],sizeof(unsigned long),MSR_LASTBRANCH_##X##_FROM_IP); \
	rsum+= pread(msr_fd,&lbrstack.msr_lastbranch_to_ip[X],sizeof(unsigned long),MSR_LASTBRANCH_##X##_TO_IP);


inline void dump_lbr()
{
	stop_lbr;
	/*
	 * Haswell has 16 pairs of MSR
	 * MSR_LASTBRANCH_x_FROM_IP
	 * MSR_LASTBRANCH_x_TO_TP
	 * Reference: 17.6.1
	 */
	int rsum=0;

	read_lbr_stack_pair(0);
	read_lbr_stack_pair(1);
	read_lbr_stack_pair(2);
	read_lbr_stack_pair(3);
	read_lbr_stack_pair(4);
	read_lbr_stack_pair(5);
	read_lbr_stack_pair(6);
	read_lbr_stack_pair(7);
	read_lbr_stack_pair(8);
	read_lbr_stack_pair(9);
	read_lbr_stack_pair(10);
	read_lbr_stack_pair(11);
	read_lbr_stack_pair(12);
	read_lbr_stack_pair(13);
	read_lbr_stack_pair(14);
	read_lbr_stack_pair(15);


	int i=0;
	printf("-----LBR(%d Bytes)----\n",rsum);
	for(i=0;i<16;i++)
	{
		printf("%p->%p\n",
			(void*)lbrstack.msr_lastbranch_from_ip[i],
			(void*)lbrstack.msr_lastbranch_to_ip[i]);
	}
	printf("-------------\n");
}
/*
 * test functions
 */
void func7()
{
	/*
	 * dump data from LBR
	 */
	dump_lbr();
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
	if(init_lbr())
	{
		return -1;
	}

	func0();
	cleanup_lbr();
	return 0;
}


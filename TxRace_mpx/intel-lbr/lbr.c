/*
 * LBR Library
 * 2015 Tong Zhang <ztong@vt.edu>
 * tested on Intel i7-4790(Haswell), Linux-3.19.4-gentoo (64bit)
 * need msr kernel module
 */

#include <lbr.h>
#include <stdio.h>

int msrfd[MAX_CPU]={0};

int start_lbr(int cpuid)
{
	int msr_fd = msrfd[cpuid];
	if(msr_fd==0)
	{
		char dev_file_name[256];
		sprintf(dev_file_name,MSR_DEV_FILE,cpuid);
		msr_fd = open(dev_file_name,O_RDWR);

		if(msr_fd==-1)
		{
			fprintf(stderr,"can not open msr file %s\n",dev_file_name);
			return -1;
		}
		msrfd[cpuid] = msr_fd;
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
	msr_lbr_select &= ~MSR_LBR_SELECT_CPL_NEQ_0_MASK;
	msr_lbr_select |= MSR_LBR_SELECT_JCC_MASK;
	msr_lbr_select &= ~MSR_LBR_SELECT_NEAR_REL_CALL_MASK;
	msr_lbr_select &= ~MSR_LBR_SELECT_NEAR_IND_CALL_MASK;
	msr_lbr_select |= MSR_LBR_SELECT_NEAR_RET_MASK;
	msr_lbr_select |= MSR_LBR_SELECT_NEAR_IND_JMP_MASK;
	msr_lbr_select |= MSR_LBR_SELECT_NEAR_REL_JMP_MASK;
	msr_lbr_select |= MSR_LBR_SELECT_FAR_BRANCH_MASK;
	pwrite(msr_fd,&msr_lbr_select,sizeof(unsigned long),MSR_LBR_SELECT);

	//close(msr_fd);
	return 0;
}


/*
 * clean up
 */
void cleanup_lbr(int cpuid)
{
	stop_lbr(msrfd[cpuid]);
	close(msrfd[cpuid]);
	msrfd[cpuid] = 0;
}

/*
 * dump LBR stack
 */

#define read_lbr_stack_pair(X) \
	rsum+= pread(msr_fd,&lbrstack->msr_lastbranch_from_ip[(-tos+X+LBR_STACK_SIZE)%LBR_STACK_SIZE],sizeof(unsigned long),MSR_LASTBRANCH_##X##_FROM_IP); \
	rsum+= pread(msr_fd,&lbrstack->msr_lastbranch_to_ip[(-tos+X+LBR_STACK_SIZE)%LBR_STACK_SIZE],sizeof(unsigned long),MSR_LASTBRANCH_##X##_TO_IP);


inline void dump_lbr(int cpuid, lbr_stack* lbrstack)
{
	stop_lbr(cpuid);
	/*
	 * Haswell has 16 pairs of MSR
	 * MSR_LASTBRANCH_x_FROM_IP
	 * MSR_LASTBRANCH_x_TO_TP
	 * Reference: 17.6.1
	 */

	int msr_fd = msrfd[cpuid];
	int rsum=0;

	/*
	 * read Top of Stack Register(TOS)
	 */
	unsigned long _tos;
	pread(msr_fd,&_tos,sizeof(unsigned long),MSR_LASTBRANCH_TOS);
	int tos = _tos & MSR_LASTBRANCH_TOS_MASK;

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
}

/*
 * print lbr
 */

void print_lbr(lbr_stack* lbrstack)
{
	int i=0;
	//printf("-----LBR(%d Bytes)----\n",rsum);
	printf("-----LBR----\n");
	for(i=0;i<16;i++)
	{
		printf("%p->%p\n",
			(void*)lbrstack->msr_lastbranch_from_ip[i],
			(void*)lbrstack->msr_lastbranch_to_ip[i]);
	}
	printf("-------------\n");
}

/*
 * reference 17.8.1 LBR Stack Enhancement
 */
void inteprete_lbr_info(lbr_stack* lbrstack)
{
	int i;
	for(i=0;i<16;i++)
	{
		printf("%p->%p\n",
			(void*)((lbrstack->msr_lastbranch_from_ip[i])&MSR_LBR_DATA_MASK),
			(void*)lbrstack->msr_lastbranch_to_ip[i]);
		int q=0;
		if(lbrstack->msr_lastbranch_from_ip[i]&MSR_LBR_MISPRED_MASK)
		{
			q=1;
			printf("MISPRED,");
		}
		if(lbrstack->msr_lastbranch_from_ip[i]&MSR_LBR_IN_TSX_MASK)
		{
			q=1;
			printf("IN_TSX,");
		}
		if(lbrstack->msr_lastbranch_from_ip[i]&MSR_LBR_IN_TSX_MASK)
		{
			q=1;
			printf("ABRT,");
		}
		if(q!=0)
			printf("\n");
	}
}

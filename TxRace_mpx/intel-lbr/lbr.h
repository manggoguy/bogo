/*
 * LBR Library Header
 * 2015 Tong Zhang <ztong@vt.edu>
 * tested on Intel i7-4790(Haswell), Linux-3.19.4-gentoo (64bit)
 * need msr kernel module
 */

#ifndef _LBR_H_
#define _LBR_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>

/*
 * Register Offsets
 */

#define MSR_LBR_SELECT (0x1C8)

#define MSR_LASTBRANCH_TOS (0x1C9)

#define MSR_LASTBRANCH_TOS_MASK (0x0F)

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

#define MSR_LBR_SELECT_CPL_EQ_0_MASK (1<<0)
#define MSR_LBR_SELECT_CPL_NEQ_0_MASK (1<<1)
#define MSR_LBR_SELECT_JCC_MASK (1<<2)
#define MSR_LBR_SELECT_NEAR_REL_CALL_MASK (1<<3)
#define MSR_LBR_SELECT_NEAR_IND_CALL_MASK (1<<4)
#define MSR_LBR_SELECT_NEAR_RET_MASK (1<<5)
#define MSR_LBR_SELECT_NEAR_IND_JMP_MASK (1<<6)
#define MSR_LBR_SELECT_NEAR_REL_JMP_MASK (1<<7)
#define MSR_LBR_SELECT_FAR_BRANCH_MASK (1<<8)


/*
 * Refer 17.8.1 LBR Stack Enhancement
 */
#define MSR_LBR_DATA_MASK (0x0000FFFFFFFFFFFF)
#define MSR_LBR_SIGN_EXT_MASK (0x1FFF000000000000)
#define MSR_LBR_TSX_ABORT_MASK (0x2000000000000000)
#define MSR_LBR_IN_TSX_MASK (0x4000000000000000)
#define MSR_LBR_MISPRED_MASK (0x8000000000000000)

/*
 * for Haswell, there are 16 pairs of LBR Message
 */

#define LBR_STACK_SIZE (16)

/*
 * MSR should be accessed from ring0
 * use kernel facility to access msr
 */

#define MSR_DEV_FILE "/dev/cpu/%d/msr"

/*
 * support up to N CPUS
 */

#define MAX_CPU 16


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct _lbr_stack
{
	unsigned long msr_lastbranch_from_ip[16];
	unsigned long msr_lastbranch_to_ip[16];
} lbr_stack;

extern int msrfd[];

/*
 * start LBR facility for cpuid
 */
int start_lbr(int cpuid);

/*
 * stop LBR facility for cpuid
 * inline? stop_lbr to minimize impact of unrelated branch
 */
 #define stop_lbr(cpuid) \
 { \
 	int msr_fd = msrfd[cpuid]; \
	unsigned long ia32_debugctl; \
	pread(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL); \
	ia32_debugctl &= ~IA32_DEBUGCTL_LBR_MASK; \
	ia32_debugctl &= ~IA32_DEBUGCTL_FRZ_LBRS_ON_PMI_MASK; \
	pwrite(msr_fd,&ia32_debugctl,sizeof(unsigned long),IA32_DEBUGCTL); \
}

/*
 * clean LBR facility for cpuid
 */
void cleanup_lbr(int cpuid);

/*
 * dump LBR info
 */

inline void dump_lbr(int cpuid, lbr_stack* lbrstack);

/*
 * Print LBR info
 */

void print_lbr(lbr_stack* lbrstack);

/*
 * inteprete LBR info
 */
void inteprete_lbr_info(lbr_stack* lbrstack);


#ifdef __cplusplus
}
#endif


#endif//_LBR_H_

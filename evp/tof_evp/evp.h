#ifndef _EVP_H_
#define _EVP_H_

#include <rts.h>

#ifdef RTS_PROJECT_PP
#define EVP_HOSTNAME	"ppdaq1.pp2pp.bnl.gov"
#else
#define EVP_HOSTNAME	"evp.starp.bnl.gov"
#endif

#define EVP_PORT	8020

#define EVP_TYPE_0	1
#define EVP_TYPE_PHYS	2
#define EVP_TYPE_SPEC	4
#define EVP_TYPE_ANY	(EVP_TYPE_PHYS|EVP_TYPE_SPEC|EVP_TYPE_0)
#define EVT_TYPE_MON	8


#define EVP_STAT_OK	0
#define EVP_STAT_EOR	(-1)
#define EVP_STAT_EVT	(-2)
#define EVP_STAT_CRIT	(-3)

// file systems and related sizes
#define EVP_FS		"/EVP/evpTask_fs.txt"
#define EVP_FS_MAX	10
#define EVP_MAX_FS_NAME	16
#define EVP_FS_MIN_BLOCKS	131072
#define EVP_FS_PRUNE_BLOCKS	(5*EVP_FS_MIN_BLOCKS)
#define EVP_FS_MIN_INODES	100


extern int sanityCheck(char *datap) ;
extern int sanityCheckStartRun(void) ;

#endif

/* $Id$ */
#ifndef __CAN_UTILS_H__
#define __CAN_UTILS_H__

extern void printCANMsg(const TPCANMsg &, const char *);
extern int openCAN(WORD);
extern int sendCAN_and_Compare(TPCANMsg &, const char *, const int);

#endif // __CAN_UTILS_H__
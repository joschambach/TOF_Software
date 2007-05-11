/* File name     : can_utils.cc
 * Creation date : 5/11/2007
 * Author        : J. Schambach, UT Physics
 * Modified date : 
 *               : 
 */

#ifndef lint
static char  __attribute__ ((unused)) vcid[] = 
"$Id: can_utils.cc,v 1.1 2007-05-11 16:50:50 jschamba Exp $";
#endif /* lint */

// #define LOCAL_DEBUG

//****************************************************************************
// INCLUDES
// C++ header file
#include <iostream>
using namespace std;

// other headers
#include <errno.h>
#include <fcntl.h>

// pcan include file
#include <libpcan.h>

//****************************************************************************
// GLOBALS
extern HANDLE h;

//****************************************************************************
// LOCALS


//****************************************************************************
// CODE 


//****************************************************************************
void printCANMsg(const TPCANMsg &msg, const char *msgTxt)
{
  printf("%s: %c %c 0x%03x %1d  ",
	 msgTxt,
	 (msg.MSGTYPE & MSGTYPE_RTR)      ? 'r' : 'm',
	 (msg.MSGTYPE & MSGTYPE_EXTENDED) ? 'e' : 's',
	 msg.ID, 
	 msg.LEN); 
  for (int i = 0; i < msg.LEN; i++)
    printf("0x%02x ", msg.DATA[i]);
  printf("\n");
}

//****************************************************************************
int openCAN(WORD devID)
{
  char devName[255];
  TPDIAG my_PDiag;
  char txt[255]; // temporary string storage

  // search for correct device ID:
  for (int i=0; i<8; i++) {
    sprintf(devName, "/dev/pcan%d", 32+i);
    h = LINUX_CAN_Open(devName, O_RDWR);
    if (h == NULL) {
      //printf("Failed to open device %s\n", devName);
      //my_private_exit(errno);
      continue;
    }
    // get the hardware ID from the diag structure:
    LINUX_CAN_Statistics(h,&my_PDiag);
    printf("\tDevice at %s: Hardware ID = 0x%x\n", devName, 
	   my_PDiag.wIrqLevel);
    if (my_PDiag.wIrqLevel == devID) break;
    CAN_Close(h);
  }

  if (!h) {
    printf("Device ID 0x%x not found\n", devID);
    errno = nGetLastError();
    perror("CAN_Open()");
    return(errno);
  }

    
  // get version info
  errno = CAN_VersionInfo(h, txt);
  if (!errno) {
#ifdef LOCAL_DEBUG
    cout << "driver version = "<< txt << endl;
#endif
  }
  else {
    perror("CAN_VersionInfo()");
    return(errno);
  }

  // open CAN Port, init PCAN-USB
  errno = CAN_Init(h, CAN_BAUD_1M,  CAN_INIT_TYPE_ST);
  if (errno) {
    perror("CAN_Init()");
    return(errno);
  } 

  return (0);
}

//****************************************************************************
int sendCAN_and_Compare(TPCANMsg &ms, const char *errorMsg, const int timeout)
{
  TPCANRdMsg mr;
  __u32 status;
#ifdef LOCAL_DEBUG
  char msgTxt[256];
#endif
  unsigned int expectedID = ms.ID + 1;

  // send the message
  if ( (errno = CAN_Write(h, &ms)) ) {
    perror("CAN_Write()");
    return(errno);
  }
  
  errno = LINUX_CAN_Read_Timeout(h, &mr, timeout);
  if (errno == 0 ) { // data received
#ifdef LOCAL_DEBUG
    sprintf(msgTxt, "%s, message received", errorMsg);
    printCANMsg(mr.Msg, msgTxt);
#endif
    // check if a CAN status is pending	     
    if (mr.Msg.MSGTYPE & MSGTYPE_STATUS) {
      status = CAN_Status(h);
      if ((int)status < 0) {
	errno = nGetLastError();
	perror("CAN_Status()");
	return(errno);
      }
      else
	cout << "pending CAN status " << showbase << hex << status << " read.\n";
    } 
    else if (mr.Msg.MSGTYPE == MSGTYPE_STANDARD) {
      // now interprete the received message:
      // check if it's a proper response
      if ( mr.Msg.ID != expectedID ) {
	cout << "ERROR: " << errorMsg 
	     << " request: Got something other than writeResponse: ID " 
	     << showbase << hex << (unsigned int)mr.Msg.ID 
	     << ", expected response to " << (unsigned int)ms.ID << endl;	
	printCANMsg(mr.Msg, "response:");
	return (-2);
      }
      if (mr.Msg.LEN != ms.LEN) { // check for correct length
	cout << "ERROR: " << errorMsg << " request: Got msg with incorrect data length " 
	     << dec << (int)mr.Msg.LEN << ", expected " << (int)ms.LEN << endl;
	cout << errorMsg << " response: first byte: " 
	     << showbase << hex << (unsigned int)mr.Msg.DATA[0] 
	     << " expected " << (unsigned int)ms.DATA[0] << endl;
	printCANMsg(mr.Msg, "response");
	return (-3);
      }
      if (mr.Msg.DATA[0] != ms.DATA[0]) {
	cout << errorMsg << " response: first byte: " 
	     << showbase << hex << (unsigned int)mr.Msg.DATA[0] 
	     << " expected " << (unsigned int)ms.DATA[0] << endl;
	printCANMsg(mr.Msg, "response:");
	return (-4);
      }
      // Message is good, continue
      
      
    } // data read
  } // data received
  else if (errno == CAN_ERR_QRCVEMPTY) {	
    cout << "ERROR: Sent " << errorMsg << " packet, but did not receive response within "
	 << timeout/1000000 << " sec" << endl;
    return (-5);
  }
  else {// other read error
    cout << "LINUX_CAN_Read_Timeout returned " << errno << endl;
    return (-6);
  }

  return 0;
}


/* File name     : p_progPLD.cc
 * Creation date : 8/17/06
 * Author        : J. Schambach, UT Physics
 * Modified date : 
 *               : 
 */

#ifndef lint
static char  __attribute__ ((unused)) vcid[] = 
"$Id$";
#endif /* lint */

// #define LOCAL_DEBUG

//****************************************************************************
// INCLUDES
// C++ header file
#include <iostream>
#include <fstream>
using namespace std;

// other headers
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
//#include <time.h>

// pcan include file
#include <libpcan.h>

// local utilities
#include "can_utils.h"

//****************************************************************************
// DEFINES
#define TDIG
#define LINE_UP "[1A[80D[0J"

//****************************************************************************
// GLOBALS
HANDLE h = NULL;


//****************************************************************************
// LOCALS

//****************************************************************************

//****************************************************************************
// CODE 
//****************************************************************************

// centralized entry point for all exits
static void my_private_exit(int error)
{
  if (h)
  {
    CAN_Close(h); 
  }
#ifdef LOCAL_DEBUG
  cout << "p_progPLD: finished (" << error << ")\n";
#endif
  exit(error);
}

// handles CTRL-C user interrupt
static void signal_handler(int signal)
{
  my_private_exit(0);
}


// everything happens here
int p_progPLD(const char *filename, int pldNum, int nodeID, WORD devID)
{
  ifstream conf;
  unsigned char confByte[256];
  int fileSize, noPages;
  
  // struct timespec timesp;
  
  cout << "Configuring PLD " << pldNum 
       << " on nodeID 0x" << hex << nodeID
       << " with filename " << filename << "...\n";
  
  errno = 0;
  
  
  conf.open(filename, ios_base::binary); // "in" is default 
  if ( !conf.good() ) {
    cerr << filename << ": file error\n";
    return -1;
  }
  
  conf.seekg(0,ios::end);
  fileSize = conf.tellg();
  noPages = fileSize/256;
  
  // timesp.tv_sec = 0;
  // timesp.tv_nsec = 1000;	// 1 ms
  
  cout << "Filesize = " << dec << fileSize << " bytes, " << fileSize/1024 << " kbytes, "
       << noPages << " pages\n";
  
  conf.seekg(0,ios::beg);    // move file pointer to beginning of file
  
  
  cout << "Starting Configuration Procedure....\n";
  
  TPCANMsg ms;
  TPCANRdMsg mr;
  
  // swallow any packets that might be present first
  errno = LINUX_CAN_Read_Timeout(h, &mr, 10000); // timeout = 10 mseconds
  
  // ***************************************************************************
  
  // ************** progPLD:Start ****************************************
  // this is a write message (msgID = 0x002)
  
  ms.MSGTYPE = MSGTYPE_STANDARD;
  ms.ID = 0x002 | (nodeID << 4);
  ms.LEN = 2;
  
  ms.DATA[0] = 0x20;
  ms.DATA[1] = pldNum;
  
  cout << LINE_UP << "Starting Bulk Erase...\n"; cout.flush();
#ifdef LOCAL_DEBUG
  printCANMsg(ms, "p_progPLD: Sending progPLD:Start command:");
#endif
  
  if ( sendCAN_and_Compare(ms, "p_progPLD: progPLD:Start", 60000000) != 0) // timeout = 60 sec
    my_private_exit(errno);
  
  cout << LINE_UP << "Bulk Erase finished...\nStarting page programming...\n";
  cout.flush();
  sleep(2);
  for (int page=0; page<noPages/2; page++) {
    int EPCS_Address = page*256;
    // read 256 bytes
    conf.read((char *)confByte, 256);
    
    bool allFF = true;
    for (int i=0; i<256; i++) {
      if(confByte[i] != 0xff) {
	allFF = false;
	break;
      }
    }
    
    if (allFF) continue;
    
    // ************** progPLD:WriteAddress ****************************************
    
    ms.DATA[0] = 0x21;	// write Address, lowest to highest byte
    ms.DATA[1] = (EPCS_Address & 0x0000FF);
    ms.DATA[2] = ((EPCS_Address & 0x00FF00) >> 8);
    ms.DATA[3] = ((EPCS_Address & 0xFF0000) >> 16);
    ms.LEN = 4;
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "p_progPLD: Sending progPLD:WriteAddress command:");
#endif
    
    // do this without response:
    if ( (errno = CAN_Write(h, &ms)) ) {
      perror("p_progPLD: CAN_Write()");
      return(errno);
    }
    errno = LINUX_CAN_Read_Timeout(h, &mr, 1000000); // timeout = 1 second
    if (errno != 0) {
      if (errno == CAN_ERR_QRCVEMPTY)
	cout << "Timeout during progPLD:WriteAddress, page " << page << endl;
      else
	cout << "CAN_Read_Timeout returned " << errno 
	     << " during progPLD:WriteAddress page, " << page << endl;
    }
    
    
    ms.DATA[0] = 0x22;
    ms.LEN = 8;
    unsigned char *tmpPtr = confByte;
    for (int i=0; i<36; i++) {
      // ************** progPLD:WriteDataBytes ************************************
      
      
      for (int j=1; j<8; j++) {
	ms.DATA[j] = *tmpPtr;
	tmpPtr++;
      }
#ifdef LOCAL_DEBUG
      printCANMsg(ms, "p_progPLD: Sending progPLD:WriteDataBytes command:");
#endif
      
      // do this without response:
      if ( (errno = CAN_Write(h, &ms)) ) {
      	perror("p_progPLD: CAN_Write()");
      	return(errno);
      }
      // waste some time, so packets aren't sent too fast
      //nanosleep(&timesp, NULL);
      //for (int j=0; j<4300000; j++) ;
      
      errno = LINUX_CAN_Read_Timeout(h, &mr, 4000000); // timeout = 4 second
      if (errno != 0) {
	const char *RED_ON_WHITE = "\033[47m\033[1;31m";
	const char *NORMAL_COLORS = "\033[0m";

	cout << RED_ON_WHITE;
	if (errno == CAN_ERR_QRCVEMPTY)
	  cout << "Timeout during progPLD:WriteDataBytes, page " << page << endl;
	else
	  cout << "CAN_Read_Timeout returned " << errno 
	       << " during progPLD:WriteDataBytes, page " << page << endl;
	cout << NORMAL_COLORS;
	return (errno);
      }
      
    }
    
    // ************** progPLD:Program256 ****************************************
    
    ms.DATA[0] = 0x23;
    ms.LEN = 5;
    for (int j=1; j<5; j++) {
      ms.DATA[j] = *tmpPtr;
      tmpPtr++;
    }
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "p_progPLD: Sending progPLD:Program256 command:");
#endif
    
    if ( sendCAN_and_Compare(ms, "p_progPLD: progPLD:Program256", 1000000) != 0) { // timeout = 1sec
      cout << "page = " << dec << page << endl;
      my_private_exit(errno);
    }
    
    //#ifdef LOCAL_DEBUG
    if(page<11) {cout << LINE_UP << "Page " << dec << page << "...\n"; flush(cout);}
    else if((page%100) == 0) {cout << LINE_UP << "Page " << dec << page << "...\n"; flush(cout);}
    //#endif
    
    
  } // for (int page
  
  cout << "Page Programming finished...\nSending asDone...\n";
  ms.DATA[0] = 0x24;
  ms.LEN = 1;
#ifdef LOCAL_DEBUG
  printCANMsg(ms, "p_progPLD: Sending progPLD:asDone command:");
#endif
  
  if ( sendCAN_and_Compare(ms, "p_progPLD: progPLD:asDone", 1000000) != 0) // timeout = 1 sec
    my_private_exit(errno);
  
  
  // ************************* finished !!!! ****************************************
  // ********************************************************************************
  
  conf.close();
  cout << "... Configuration finished successfully.\n";
  
  return errno;
}


// *********************** main function *********************************************
int main(int argc, char *argv[])
{
  WORD devID = 255;
  int returnVal;

  cout << vcid << endl;
  cout.flush();
  
  if ( argc < 4 ) {
    cout << "USAGE: " << argv[0] << " <fileName> <PLD #> <nodeID> [<devID>]\n";
    return 1;
  }
  
  // 1 <= pldNum <= 8: Serdes FPGA <pldNum>
  // pldNum = 0: Master FPGA
  // pldNum = 9: all Serdes FPGAs
  int pldNum = atoi(argv[2]);
  int nodeID = strtol(argv[3], (char **)NULL, 0);
  if (argc >= 5) {
    devID = strtol(argv[4],(char **)NULL, 0);
    if (devID > 255) {
      printf("Invalid Device ID 0x%x. Use a device ID between 0 and 255\n", devID);
      return -1;
    }
  }

  // install signal handler for manual break
  signal(SIGINT, signal_handler);

  if (argc == 6) {
    if((errno = openCAN_br(devID, CAN_BAUD_1M)) != 0) {
      my_private_exit(errno);
    }
  }
  else {
    // open CANbus device devID
    if((errno = openCAN(devID)) != 0) {
      my_private_exit(errno);
    }
  }

  if ((pldNum < 9) && (pldNum >= 0))
    returnVal = p_progPLD(argv[1], pldNum, nodeID, devID);
  else if (pldNum == 9)
    for (int i=1; i<9; i++)
      returnVal = p_progPLD(argv[1], i, nodeID, devID);
  else 
    cerr << "Invalid pldNum " << pldNum << "; exiting ...\n";

  my_private_exit(errno);
  return 0;
}

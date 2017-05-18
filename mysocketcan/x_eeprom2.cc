/* File name     : x_eeprom2.cc
 * Creation date : 7/2/2012
 * Author        : J. Schambach, UT Physics
 * Modified date : 
 *               : 
 */

#ifndef lint
static char  __attribute__ ((unused)) vcid[] = 
"$Id: x_eeprom2.cc 773 2012-07-25 17:32:27Z jschamba $";
#endif /* lint */


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
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>


// pcan include file
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

// local utilities
#include "can_utils.h"

//****************************************************************************
// DEFINES
// #define LOCAL_DEBUG
#define LINE_UP "[1A[80D[0J"
//#define LINE_UP ""

//****************************************************************************
// GLOBALS
int h = -1;

//****************************************************************************
// LOCALS

//****************************************************************************
// CODE 

// centralized entry point for all exits
static void my_private_exit(int error)
{
  if (h > 0)
  {
    close(h); 
  }
#ifdef LOCAL_DEBUG
  cout << "x_eeprom2: finished (" << error << ")\n";
#endif
  exit(error);
}

// handles CTRL-C user interrupt
static void signal_handler(int signal)
{
  my_private_exit(0);
}



//**********************************************
// here all is done
//*********************************************
int eeprom2(const char *filename, 
	    unsigned int tdigNodeID, 
	    unsigned int tcpuNodeID, 
	    int devID)
{
  int i,j;

  ifstream conf;
  unsigned char confByte[256];
  int fileSize, noPages, leftover;

  cout << "Programming EEPROM2"
       << " at TDIG NodeID 0x" << hex << tdigNodeID
       << " through TCPU NodeID 0x" << tcpuNodeID
       << "\n with filename " << filename 
       << " on CANbus ID 0x" << devID << "...\n";

  errno = 0;
  
  // open file "filename" and extract control bits
  conf.open(filename, ios_base::binary); // "in" is default 
  if ( !conf.good() ) {
    cerr << filename << ": file error\n";
    return -1;
  }
  
  conf.seekg(0,ios::end);    //move file pointer to beginning of file
  fileSize = conf.tellg();
  noPages = fileSize/256;
  leftover = fileSize%256;

  if (leftover > 0) noPages++;

  cout << "Filesize = " << dec 
       << fileSize << " bytes, " << fileSize/1024 << " kbytes, "
       << noPages << " pages, leftover " << leftover << " bytes\n";
  
  conf.seekg(0,ios::beg);    // move file pointer to beginning of file

  cout << "Starting Configuration Procedure....\n";

  can_frame ms;
  can_frame mr;


  // swallow any packets that might be present first
  errno = CAN_Read_Timeout(h, &mr, 100000); // timeout = 100 mseconds
  

  // ************** CONFIGURE_TDC:Write FPGA_CONFIG0 ****************************************
  ms.can_id = (0x002 | (tdigNodeID<<4)) <<18 | tcpuNodeID | CAN_EFF_FLAG;
  ms.can_dlc = 1;
  
  // "FPGA_CONFIG0"
  ms.data[0] = 0x11;
  
#ifdef LOCAL_DEBUG
  printCANMsg(ms, "x_eeprom2: Sending Write FPGA_CONFIG0 command:");
#endif
  
  if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write FPGA_CONFIG0", 500000, 2, true) != 0) // timeout = 0.5 sec
    cout << "Old MCU Code?\n\n";
  


  // NOW START; Attempt to continue, even if error in previous CANbus message, since it
  // could be old MCU code
  for (int page=0; page<noPages; page++) {
  
    int eeprom_addrs = page*256;
    // read 256 bytes
    memset((void *)confByte, 0xff, 256);
    conf.read((char *)confByte, 256);

    // ************** CONFIGURE_TDC:Write Block Start ****************************************
    ms.can_id = (0x002 | (tdigNodeID<<4)) <<18 | tcpuNodeID | CAN_EFF_FLAG;
    ms.can_dlc = 1;
    
    // "Write Block Start"
    ms.data[0] = 0x10;
    
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "x_eeprom2: Sending Write Block Start command:");
#endif
    
    if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write Block Start", 4000000, 2, true) != 0) // timeout = 4 sec
      my_private_exit(errno);
    
    
    // *************** "Write Block Data", 36 messages with 7 bytes each ***********
    ms.can_dlc = 8;
    
    unsigned char *tmpPtr = confByte;
    ms.data[0] = 0x20;
    for (i=0; i<36; i++) {
      for (j=1; j<8; j++) {
	ms.data[j] = *tmpPtr;
	tmpPtr++;
      }
      
#ifdef LOCAL_DEBUG
      printCANMsg(ms, "x_eeprom2: Sending Write Block Data packet:");
#endif
      
      
      if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write Block Data", 4000000, 2, true) != 0) // timeout = 4 sec
	my_private_exit(errno);
      
    } // end "for(i=1, ... " loop
    
    
    // **************** "Write Block Data", ... and 1 last message with 4 bytes ******
    ms.can_dlc = 5;
    
    for (j=1; j<5; j++) { 
      ms.data[j] = *tmpPtr;
      tmpPtr++;
    }
    
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "x_eeprom2: Sending Write Block Data packet 37:");
#endif
    
    
    if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write Block Data 37:", 4000000, 2, true) != 0) // timeout = 4 sec
      my_private_exit(errno);
    
    // *************************** Write Block End *************************
    ms.can_dlc = 1;
    ms.data[0] = 0x30;
    
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "x_eeprom2: Sending Write Block End packet:");
#endif
    
    
    if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write Block End:", 4000000, 8, true) != 0) // timeout = 4 sec
      my_private_exit(errno);
    
    
    // ****************************** "Write Block Target" **********************
    ms.can_dlc = 6;
    ms.data[0] = 0x4e;
    ms.data[1] = (unsigned char) eeprom_addrs & 0xFF;    // LSByte of address
    ms.data[2] = (unsigned char)(eeprom_addrs >>8) & 0xFF;    // 2nd Byte of address
    ms.data[3] = (unsigned char)(eeprom_addrs >>16) & 0xFF;   // 3rd Byte of address
    ms.data[4] = (unsigned char)(eeprom_addrs >>24) & 0xFF;   // MSByte of address
    // See if need to request "erase sector" before write (e.g. Sector Address is xx00xx)
    ms.data[5] = 0;     // Assume no erase needed on others
    if ( (eeprom_addrs & 0x00FF00) == 0) {
      ms.data[5] = 1;        // if erase needed on boundary
      // cout << " w/Erase";
    }
    
#ifdef LOCAL_DEBUG
    printCANMsg(ms, "x_eeprom2: Sending Write Block Target packet:");
#endif
    
    if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write Block Target:", 4000000, 2, true) != 0) // timeout = 4 sec
      my_private_exit(errno);
    
    if(page<11) {cout << LINE_UP << "Page " << dec << page << "...\n"; flush(cout);}
    else if((page%100) == 0) {cout << LINE_UP << "Page " << dec << page << "...\n"; flush(cout);}
  } // for (int page=0 ...


  // ************************* finished !!!! ****************************************
  // ********************************************************************************




  // ************** CONFIGURE_TDC:Write FPGA_CONFIG1 ****************************************
  ms.can_id = (0x002 | (tdigNodeID<<4)) <<18 | tcpuNodeID | CAN_EFF_FLAG;
  ms.can_dlc = 1;
  
  // "FPGA_CONFIG1"
  ms.data[0] = 0x12;
  
#ifdef LOCAL_DEBUG
  printCANMsg(ms, "x_eeprom2: Sending Write FPGA_CONFIG1 command:");
#endif
  
  if ( sendCAN_and_Compare(h, ms, "x_eeprom2: Write FPGA_CONFIG1", 1000000, 2, true) != 0) { // timeout = 1 sec
    cout << "Old MCU code?\n";
  }

  cout << "... Configuration finished successfully.\n";
  fprintf(stderr, "successfully configured EEPROM2 TDIG 0x%x through TCPU 0x%x devID %d\n",
	  tdigNodeID, tcpuNodeID, devID); fflush(stderr);

  return errno;
}


//*********************************** main function **************************************
int main(int argc, char *argv[])
{
  int devID = 0;

  cout << vcid << endl;
  cout.flush();
  
  if ( argc < 4 ) {
    cout << "USAGE: " << argv[0] << " < TDIG node ID> <TCPU node ID> <fileName> [<devID>]\n";
    return 1;
  }
  
  unsigned int tdigNodeID = strtol(argv[1], (char **)NULL, 0);
  unsigned int tcpuNodeID = strtol(argv[2], (char **)NULL, 0);

  if (argc >= 5) {
    devID = strtol(argv[4],(char **)NULL, 0);
    if (devID > 7) {
      printf("Invalid Device ID 0x%x. Use a device ID between 0 and 7\n", devID);
      return -1;
    }
  }
  
  // install signal handler for manual break
  signal(SIGINT, signal_handler);

  if((h = CAN_Open(devID)) < 0) {
    my_private_exit(-1);
  }
  
  if (tcpuNodeID != 0xff)
    if(tdigNodeID == 0xff)
      for (unsigned int i = 0x10; i<0x18; i++)
	eeprom2(argv[3], i, tcpuNodeID, devID);
    else
      eeprom2(argv[3], tdigNodeID, tcpuNodeID, devID);
  else {
    // for nodeID = 0xff, do all TCPUs serially
    vector<unsigned int> tcpuIDs;
    vector<unsigned int>::iterator it;
   
    int numTCPUs = findAllTCPUs(h, &tcpuIDs);
    if (numTCPUs > 0) {
      cout << "found " << numTCPUs << " TCPUs on this network. Starting...\n";
      for (it=tcpuIDs.begin(); it<tcpuIDs.end(); it++)
	if(tdigNodeID == 0xff)
	  for (unsigned int i = 0x10; i<0x18; i++)
	    eeprom2(argv[3], i, *it, devID);
	else
	  eeprom2(argv[3], tdigNodeID, *it, devID);
    }
    else
      cout << "findAllTCPUs returned " << numTCPUs << ". Exiting...\n";
  }

  my_private_exit(errno);
  return 0;
}

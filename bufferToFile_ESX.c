/*********************************************************
 * Copyright 2014 VMware, Inc.  All rights reserved.
 * -- VMware Confidential
 *********************************************************/

/*
 * bufferToFile_ESX.c
 *
 *    This file contains the function which converts a buffer containing
 *    MD5 hashes of VSI components into a text based dump which can be 
 *    utilized for deduplication analysis
 *
 */


#include <stdio.h>
#include <string.h>
#include "str.h"
#define DST "/testOutDir/dump2"
#define MAX_BLKS 1024
#define SIZE_BLK 4096 // Each block is 4K

/*
 * -----------------------------------------------------------
 *
 * bufferToFile --
 *
 *    Uses data passed to append to a dump file which can be 
 *    used for deduplication analysis
 *
 * Side Effect:
 *    File contents for dump file change.
 *
 * ------------------------------------------------------------
 */

unsigned long
   bufferToFile(char *buf,  // IN: Buffer containing hex-formatted MD5 values
		  int len,    // IN: Length of the buffer containing MD5 values
		  char *UID,  // IN: Unique ID of component
		  unsigned int callNo)	// IN: Call number with same UID
{
   char hash[16 * 2 + 1];  // Each bytes represeneted by 2 hex chars
			   // and terminted by a null character
   int buf_pos;
   FILE* op;
   unsigned long offset;
   offset = MAX_BLKS * SIZE_BLK * (unsigned long)(callNo - 1);
   op = fopen(DST,"ab");
   buf_pos = 0;
   while(buf_pos < len){
      Str_Strcpy(hash, buf + buf_pos, 32);
      hash[16 * 2] = '\0'; // Null character to end string
      fprintf(op, "%s,%s,%lu\n", UID, hash, offset);
      offset += SIZE_BLK;
      buf_pos += 32;
   }
   fclose(op);
   return offset;
}

/*
 * ---------------------------------------------------------
 *
 *  testCase --
 *
 *    Provides a test case to test function 'bufferToFile'
 *
 * ---------------------------------------------------------
 */

void testCase(){
   char buf[100];
   char uid[100];
   Str_Strcpy(buf,"1234567891123456123456789012345621345678911234561234567890123456",64);
   Str_Strcpy(uid,"abc",3);
   bufferToFile(buf,64,uid,1);
}

int main(){
   testCase();
   return 0;
}

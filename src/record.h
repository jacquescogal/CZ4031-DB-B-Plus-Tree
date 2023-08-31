/*
 * Structures.h
 *
 *  Created on: 1 Mar 2023
 *      Author: jacques
 */

#ifndef RECORD_H_
#define RECORD_H_

// Structure for a record to store data of fields
// 19 bytes
// data alignment by the compiler for 64 bit system
// sizeof(Record) is 20B due to padding for byte alignment depending on CPU
struct Record
{
	//Max length of tConst is 10. 11 is used to fit null terminator
	// 11 bytes
	char tConst[11];
	//Good up to 7 decimal and is 4 bytes
	// 4 bytes
	// 0-10
	// max 4 decimal places
	float averageRating;
	// no negative numbers and max numVotes of 2,279,223<2**32
	// 4 bytes
	unsigned int numVotes;
};



#endif /* RECORD_H_ */

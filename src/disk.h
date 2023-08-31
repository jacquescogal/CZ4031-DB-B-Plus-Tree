/*
 * disk.h
 *
 *  Created on: 1 Mar 2023
 *      Author: jacqu
 */

#ifndef DISK_H_
#define DISK_H_

#include <vector>
#include <set>
#include <map>
#include "address.h"
#include "block.h"

class Disk {
private:
	std::set<Block*> blockSet; //Has all blocks
	std::map<Block*,bool> blockMap; //Quickly get blocks with gaps
	short RECORD_SIZE; //Size of a record
	size_t DISK_SIZE; //Size of the disk
	size_t maxBlocks; //maximum number of blocks in disk

public:
	Disk(std::size_t DISK_SIZE,short BLOCK_SIZE,short RECORD_SIZE); //Constructor
	Address insertRecord(Record record); //Insert the record into ablock
	void deleteRecord(Address address); //delete a record using address
	void deleteRecord(unsigned int numVotes,size_t* dataBlocksAccessed); //delete a records using key
	std::vector<Record*> getRecords(unsigned int numVotes,size_t* dataBlocksAccessed); //get records based on key
	std::vector<Record*> getRecordsInRange(unsigned int numVotesLower,unsigned int numVotesUpper,size_t* dataBlocksAccessed); //get records base on key range
};

#endif /* DISK_H_ */

/*
 * disk.cpp
 *
 *  Created on: 1 Mar 2023
 *      Author: jacqu
 */

#include <vector>
#include <iostream>
#include <math.h>
#include <stack>
#include "disk.h"

using namespace std;

Disk::Disk(std::size_t DISK_SIZE,short BLOCK_SIZE,short RECORD_SIZE) {
	this->RECORD_SIZE=RECORD_SIZE;
	this->DISK_SIZE=DISK_SIZE;
	this->maxBlocks=floor(DISK_SIZE/BLOCK_SIZE);
}


Address Disk::insertRecord(Record record){
	if (blockSet.size()>=maxBlocks){
cout<<"Out of storage"<<endl;
throw;
	}
	Address recordAddress;
	if (blockMap.size()<1){
		Block* aBlock=new Block(RECORD_SIZE);
		recordAddress = aBlock->addRecord(record);
		blockSet.insert(aBlock);
		if (!aBlock->isFull()) blockMap[aBlock]=true; // has space
	}
	else {
		Block* aBlock=blockMap.begin()->first;
		recordAddress = aBlock->addRecord(record);
		if (aBlock->isFull()) blockMap.erase(aBlock); //no space
	}
	return recordAddress;
}

void Disk::deleteRecord(Address address){
	Block* aBlock=(Block*)address.blockPtr;
aBlock->delRecord(address.offset);
if(aBlock->isEmpty()){
blockMap.erase(aBlock);
blockSet.erase(aBlock);
}
else{
blockMap[aBlock]=true;
}
}

std::vector<Record*> Disk::getRecords(unsigned int numVotes,size_t* dataBlocksAccessed){
	vector<Record*> recordsVector;
	for (auto block:blockSet){
		++(*dataBlocksAccessed);
		block->getRecordsByKey(recordsVector, numVotes);
	}
}

std::vector<Record*> Disk::getRecordsInRange(unsigned int numVotesLower,unsigned int numVotesUpper,size_t* dataBlocksAccessed){
	vector<Record*> recordsVector;
	for (auto block:blockSet){
		++(*dataBlocksAccessed);
		block->getRecordsByKeyInRange(recordsVector, numVotesLower,numVotesUpper);
	}
}

void Disk::deleteRecord(unsigned int numVotes,size_t* dataBlocksAccessed){
	for (auto block:blockSet){
			++(*dataBlocksAccessed);
			block->deleteRecordsByKey(numVotes);
		}
}


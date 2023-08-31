/*
 * block.h
 *
 *  Created on: 1 Mar 2023
 *      Author: jacques
 */

#ifndef BLOCK_H_
#define BLOCK_H_
#include <queue>
#include <set>
#include "address.h"
#include "record.h"

class Block {
private:
	std::queue<short> recordQ; //offset Q'd if empty
	std::set<short>recordSet; //offset in set if not deleted
	short BLOCK_SIZE=200; //Total block size
	short maxRecordNum; //max Records in one block
	Record* recordPointer; //Points to dynamic array
public:
	Block(short recordSize); //Constructor
	Address addRecord(Record record); //Adds a record to the block up to limit
	void delRecord(short offset); //Marks record for deletion
	void printAll(); // Prints all records in the recordset
	bool isFull(); //Checks if max record num has been reached
	bool isEmpty(); //Checks if empty, if so, free from disk
	Record* getRecord(short offset); //Return Record pointer based on offset of block
	void getRecordsByKey(std::vector<Record*> &recordsVector,unsigned int numVotes); //Stores record into recordsVector according to key
	void getRecordsByKeyInRange(std::vector<Record*> &recordsVector,unsigned int numVotesLower,unsigned int numVotesUpper); //Stores record into recordsVector according to key in range
	void deleteRecordsByKey(unsigned int numVotes);//Deletes Record based off key
};

#endif /* BLOCK_H_ */

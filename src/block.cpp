/*
 * block.cpp
 *
 *  Created on: 1 Mar 2023
 *      Author: jacqu
 */

#include <iostream>
#include <math.h>
#include "block.h"

using namespace std;
Block::Block(short recordSize) {
	this->recordPointer=(Record*)malloc(floor(this->BLOCK_SIZE/recordSize)*sizeof(Record));
	this->maxRecordNum=floor(this->BLOCK_SIZE/recordSize);
	for (short i=0;i<maxRecordNum;++i){
		recordQ.push(i);
	}
}

Address Block::addRecord(Record record){
	short offset=recordQ.front();
	recordSet.insert(offset);
	recordQ.pop();
	*(recordPointer+offset)=record;
	return{this,offset};
}

void Block::delRecord(short offset){
	recordQ.push(offset);
	recordSet.erase(offset);
}

Record* Block::getRecord(short offset){
	return (recordPointer+offset);
}

void Block::printAll(){
	for (short i=0;i<maxRecordNum;++i){
		cout<<(recordPointer+i)->tConst<<endl;
		cout<<(recordPointer+i)->averageRating<<endl;
		cout<<(recordPointer+i)->numVotes<<endl;
	}
}

bool Block::isFull(){
	if (recordQ.size()>0)return false;
	else return true;
}

bool Block::isEmpty(){
	if (recordQ.size()==maxRecordNum) return true;
	else return false;
}

void Block::getRecordsByKey(vector<Record*> &recordsVector,unsigned int numVotes){
	for (short offset:recordSet){
		Record* record=getRecord(offset);
		if (record->numVotes==numVotes)recordsVector.push_back(record);
	}
}

void Block::getRecordsByKeyInRange(vector<Record*> &recordsVector,unsigned int numVotesLower,unsigned int numVotesUpper){
	for (short offset:recordSet){
		Record* record=getRecord(offset);
		if (record->numVotes>=numVotesLower && record->numVotes<=numVotesUpper)recordsVector.push_back(record);
	}
}

void Block::deleteRecordsByKey(unsigned int numVotes){
	for (short offset:recordSet){
			Record* record=getRecord(offset);
			if (record->numVotes==numVotes)delRecord(offset);
		}
}

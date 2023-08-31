//============================================================================
// Name        : database_project_1.cpp
// Author      : Jacques
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <math.h>
#include <numeric>
#include "disk.h"
#include "record.h"
#include "block.h"
#include "treenode.h"
#include "address.h"
#include "bptree.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;

double timeThis(){
	auto start=std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double,milli> diff4 = end - start;
	return diff4.count();
}


void experiment3(BPTree* bpTree,Disk* disk){
	cout<<"================Experiment 3================"<<endl;
		size_t* indexNodesAccessed=new size_t(0);
		size_t* dataBlocksAccessed=new size_t(0);

		auto start = std::chrono::system_clock::now();

		vector<Record*> recordsVectorTree=bpTree->getRecords(500,indexNodesAccessed,dataBlocksAccessed);

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double,milli> diff = end - start;

		double sumAvg=0;
		for (Record* i:recordsVectorTree){
			sumAvg+=i->averageRating;
		}

		size_t* dataBlocksAccessed_brute=new size_t(0);

		start=std::chrono::system_clock::now();
		disk->getRecords(500,dataBlocksAccessed_brute);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double,milli> diff2 = end - start;




		cout<<"The number of index nodes the process accesses is "<<*indexNodesAccessed<<endl;
		cout<<"The number of data blocks the process accesses is "<<*dataBlocksAccessed<<endl;
		cout<<"The average of \"averageRating's\" is "<<sumAvg/recordsVectorTree.size()<<endl;
		cout<<"The running time of the retrieval process is "<<diff.count()<<" ms"<<endl;

		cout<<"The number of data blocks the linear scan process accesses is "<<*dataBlocksAccessed_brute<<endl;
		cout<<"The running time of the linear scan retrieval process is "<<diff2.count()<<" ms"<<endl;
		cout<<"============================================"<<endl;
		delete dataBlocksAccessed_brute;
		delete indexNodesAccessed;
		delete dataBlocksAccessed;
}

void experiment4(BPTree* bpTree,Disk* disk){
	cout<<"================Experiment 4================"<<endl;
	//) Experiment 4: retrieve those movies with the attribute
	//“numVotes” from 30,000 to 40,000, both inclusively and report the following
	//statistics:
	//	• the number of data blocks the process accesses;
	//	• the average of “averageRating’s” of the records that are returned;
	//	• the running time of the retrieval process;
	//	• the number of data blocks that would be accessed by a brute-force
	//	linear scan method (i.e., it scans the data blocks one by one) and its
	//	running time (for comparison)

		size_t* indexNodesAccessed=new size_t(0);
		size_t* dataBlocksAccessed=new size_t(0);

		auto start=std::chrono::system_clock::now();
		vector<Record*> recordsVectorTree=bpTree->getRecordsInRange(30000,40000,indexNodesAccessed,dataBlocksAccessed);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double,milli> diff = end - start;
		double sumAvg=0;
		for (Record* i:recordsVectorTree){
			sumAvg+=i->averageRating;
		}


		size_t* dataBlocksAccessed_brute=new size_t(0);
		start=std::chrono::system_clock::now();
		disk->getRecordsInRange(30000,40000,dataBlocksAccessed_brute);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double,milli> diff2 = end - start;


		cout<<"The number of index nodes the process accesses is "<<*indexNodesAccessed<<endl;
		cout<<"The number of data blocks the process accesses is "<<*dataBlocksAccessed<<endl;
		cout<<"The average of \"averageRating's\" is "<<sumAvg/recordsVectorTree.size()<<endl;
		cout<<"The running time of the retrieval process is "<<diff.count()<<" ms"<<endl;


		cout<<"The number of data blocks the linear scan process accesses is "<<*dataBlocksAccessed_brute<<endl;
		cout<<"The running time of the linear scan retrieval process is "<<diff2.count()<<" ms"<<endl;
		cout<<"============================================"<<endl;
		delete dataBlocksAccessed_brute;
		delete indexNodesAccessed;
		delete dataBlocksAccessed;
}

void experiment5(BPTree* bpTree,Disk* disk){

	cout<<"================Experiment 5================"<<endl;
//
//	the number nodes of the updated B+ tree;
//	• the number of levels of the updated B+ tree;
//	CE4031-CZ4031
//	• the content of the root node of the updated B+ tree(only the keys);
//	• the running time of the process;
//	• the number of data blocks that would be accessed by a brute-force
//	linear scan method (i.e., it scans the data blocks one by one) and its
//	running time (for comparison)
	size_t* indexNodesAccessed=new size_t(0);
	size_t* dataBlocksAccessed=new size_t(0);
	size_t numberOfRecords=bpTree->getRecords(1000,indexNodesAccessed,dataBlocksAccessed).size();

	auto start=std::chrono::system_clock::now();
	bpTree->deleteRecord(1000,disk);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double,milli> diff = end - start;



	cout<<"The number of nodes of the B+ tree is "<<bpTree->getNodeCount()<<endl;
	cout<<"The number of levels of the B+ tree is "<<bpTree->getLevelCount()<<"(0-indexed)"<<endl;
	cout<<"The Content of the root:"<<endl;
	bpTree->printRoot();
	cout<<"The running time of the deletion process is "<<diff.count()<<" ms"<<endl;

	for (int i=0;i<numberOfRecords;++i){
		Record record;
		record.numVotes=1000;
		Address assigned=disk->insertRecord(record);
		bpTree->insertRecord(record.numVotes,assigned);
	}

	size_t* dataBlocksAccessed_brute=new size_t(0);
		start=std::chrono::system_clock::now();
		disk->deleteRecord(1000,dataBlocksAccessed_brute);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double,milli> diff2 = end - start;


	cout<<"The number of data blocks the linear scan process accesses is "<<*dataBlocksAccessed_brute<<endl;
	cout<<"The running time of the linear scan retrieval process is "<<diff2.count()<<" ms"<<endl;



	cout<<"============================================"<<endl;
}



int main() {
	//============================================================================
	// Defining constants
	//============================================================================
	const short ADDRESS_SIZE=sizeof(Address); //Depends on OS and Architecture
	const short BLOCK_SIZE=200; //200 Bytes
	const size_t DISK_SIZE=500*pow(2,20); //500MB

	//For a memory of size 500MB (Max Size) with block size of 200B
	// The max index to be represented is 2,500,000
	// 2,500,000<unsigned int(range(0,2^32)==0 to 4,294,967,295)
	const short KEY_SIZE=sizeof(unsigned int);
	const short RECORD_SIZE=sizeof(Record);

	cout<<"================INITIALIZATION================"<<endl;
	cout<<"Storage details:"<<endl;
	cout<<"Record size is "<<RECORD_SIZE<<"B"<<endl;
	cout<<"Block size is "<<BLOCK_SIZE<<"B"<<endl;
	cout<<"Disk size is "<<floor(DISK_SIZE/pow(2,20))<<"MB"<<endl;
	cout<<"Max records in one block "<<floor(BLOCK_SIZE/RECORD_SIZE)<<endl;
	cout<<"Max blocks in disk "<<floor(DISK_SIZE/BLOCK_SIZE)<<endl;
	cout<<endl;
	cout<<"B+ Tree details:"<<endl;
	cout<<"Node size is "<<BLOCK_SIZE<<"B"<<endl;
	cout<<"Key size is "<<KEY_SIZE<<"B"<<endl;
	cout<<"Address size is "<<ADDRESS_SIZE<<"B"<<endl;
	cout<<"Max keys in one node "<<floor((BLOCK_SIZE-ADDRESS_SIZE)/(ADDRESS_SIZE+KEY_SIZE))<<endl;
	cout<<"Max pointers in one node "<<floor((BLOCK_SIZE-ADDRESS_SIZE)/(ADDRESS_SIZE+KEY_SIZE))+1<<endl;
	cout<<endl;
	cout<<"Linked List details:"<<endl;
	cout<<"Address size is "<<ADDRESS_SIZE<<"B"<<endl;
	cout<<"Node size is "<<BLOCK_SIZE<<"B"<<endl;
	cout<<"Max pointers in one node is "<<floor((BLOCK_SIZE)/(ADDRESS_SIZE))<<endl;
	cout<<"=============================================="<<endl;

	Disk* disk=new Disk(DISK_SIZE,BLOCK_SIZE,RECORD_SIZE);
	BPTree* bpTree=new BPTree(floor((BLOCK_SIZE-ADDRESS_SIZE)/(ADDRESS_SIZE+KEY_SIZE)));


	// input file stream
	ifstream file("data.tsv");


	int i=0;
	if (file.is_open()) {
		string line;

		// Consume title row
		getline(file, line);

		while (getline(file, line))  {
			istringstream iss(line);
			Record aRecord;
			iss>>aRecord.tConst>>aRecord.averageRating>>aRecord.numVotes;
			Address assigned=disk->insertRecord(aRecord);

			bpTree->insertRecord(aRecord.numVotes,assigned);
			++i;
		}
		file.close();
	} else {
		cout << "File is missing" << endl;
	}

	cout<<"================Experiment 1================"<<endl;
	cout<<"Number of records is "<<i<<endl;
	cout<<"The size of a record is "<<RECORD_SIZE<<"B"<<endl;
	cout<<"The number of records stored in a block is "<<floor(BLOCK_SIZE/RECORD_SIZE)<<endl;
	cout<<"The number of blocks for storing the data is "<<ceil(i/floor(BLOCK_SIZE/RECORD_SIZE))<<endl;
	cout<<"============================================"<<endl;

	cout<<"================Experiment 2================"<<endl;
	cout<<"The parameter n is "<<floor((BLOCK_SIZE-ADDRESS_SIZE)/(ADDRESS_SIZE+KEY_SIZE))<<endl;
	cout<<"The number of nodes of the B+ tree is "<<bpTree->getNodeCount()<<endl;
	cout<<"The number of levels of the B+ tree is "<<bpTree->getLevelCount()<<"(0-indexed)"<<endl;
	cout<<"The Content of the root:"<<endl;
	bpTree->printRoot();
	cout<<"============================================"<<endl;

	experiment3(bpTree,disk);
	experiment4(bpTree,disk);
	experiment5(bpTree,disk);




	return 0;
}

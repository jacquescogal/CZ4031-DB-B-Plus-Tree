/*
 * bptree.h
 *
 *  Created on: 3 Mar 2023
 *      Author: jacques
 */

#ifndef BPTREE_H_
#define BPTREE_H_
#include "treenode.h"
#include "record.h"
#include "disk.h";
class BPTree {
private:
	TreeNode* root; //Pointer to the root node
	TreeNode* nPtr; //Pointer to a node
	short maxNodeKey; //max number of keys in node
unsigned int nodeCount; //number of nodes in bpTree
public:
	BPTree(short maxNodeKey); //constructor
	virtual ~BPTree();
	void insertRecord(unsigned int numVotes,Address address); //insert Record into bp tree with address and key
	void deleteRecord(unsigned int numVotes,Disk* disk); //delete Record from bp tree with key
	unsigned int getNodeCount(); //get Node count
	void printRoot(); //print the root only
	void printAll(); //print the whole bp tree
	std::vector<Record*> getRecords(unsigned int numVotes,size_t* indexNodesAccessed,size_t* dataBlocksAccessed);//get records vector with key
	std::vector<Record*> getRecordsInRange(unsigned int numVotesLower,unsigned int numVotesUpper,size_t* indexNodesAccessed,size_t* dataBlocksAccessed); //get records vector with key range
	short getLevelCount(); //return the number of levels in bp tree not counting the linked list
};

#endif /* BPTREE_H_ */

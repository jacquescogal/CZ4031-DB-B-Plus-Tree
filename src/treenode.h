/*
 * treenode.h
 *
 *  Created on: 3 Mar 2023
 *      Author: jacqu
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include "record.h"
#include "address.h"
#include "disk.h"
#include <string>
#include <vector>
#include <stack>
#include <set>
class TreeNode {
private:
	unsigned int* keyPtr; //Dynamic array for key (numVotes)
	Address* aPtr; //Dynamic array for address
	short keyNum; //Current number and also offset to next memlocation for new data
	short ptrNum; //Current number and also offset to next memlocation for new data
	short maxKey; //Maximum Key Num to be checked against
	short maxPtr; //Maximum Add Num to be checked against
	short h; //Height of the tree node
	bool l; //Leaf or not
	bool neighbourPtr; //Whether there is a neighbour being pointed to
	unsigned int minKey; //What is the minimum key of the tree node and children
public:
	TreeNode(short maxKey,bool isLeaf,short height); //constructor overloaded
	TreeNode(short maxPtr); //constructor
	virtual ~TreeNode();
	bool isFull(); //Check if max key reached
	bool isLeaf(); // returns l
	void insertRecord(unsigned int numVotes, Address address); //insert record
	void insertRecord(unsigned int numVotes, Address address, bool copy); //insert record and address as plain copy
	void insertRecord(unsigned int numVotesLeft, Address addressLeft,unsigned int numVotesRight,Address addressRight); //Insert record to new neighbour
	void deleteRecord(unsigned int numVotes,Disk* disk); //Delete Record of key NumVote
bool hasKey(unsigned int numVotes); //Check if the treenode has exact key
	TreeNode* llInsertRecord(Address address); // Insert record into link list
	bool llIsFull(); //Check if linked list address space is full
	void llPrint(); //Print contents of linked list for debugging
	void llDeleteChain(Disk* disk); //Triggers delete chain to remove all instances of record with key NumVotes
	void llSetNeighbour(TreeNode* listNode);//Sets the neighbour to be pointed to by the linked list
	void llRetrieveChain(std::vector<Record*> &vectorRecord,size_t* indexNodesAccessed,std::set<Block*> &blkPtrSet); // Recursive retrieval of all addresses in linked list
	void printAll(); //Prints all members
	void setNeighbour(TreeNode* listNode); //Sets neighbour
	void splitInsert(unsigned int numVotes,TreeNode* siblingNode,Address address); //split then insert into right sibling
	unsigned int getMinKey(); //return minKey
	TreeNode* goNext(unsigned int numVotes); //Returns the next address to follow
	TreeNode* goNext(unsigned int numVotes,std::stack<short> &offsetStack); //Returns the next address to follow and the offset for the parent node
	std::string getStringKeys(); //Gets the keys as a string
	short getHeight(); //returnsh
	std::vector<TreeNode*> getAllAddress(); //Returns all the addresses of the children
	std::vector<Record*> getRecordsOfKey(unsigned int numVotes,size_t* indexNodesAccessed,size_t* dataBlocksAccessed); //Returns a vector of the records for given key
	void storeRecordsOfKeyUpToLimit(std::vector<Record*> &recordsVector,std::set<Block*> &blockPtrSet,unsigned int numVotesLower,unsigned int numVotesUpper,size_t* indexNodesAccessed);
	bool isUnderflow(bool root); //Check if the there is an underflow for the node.

	//Checked on deletion
	bool hasLeftSibling(unsigned int numVotes); //Check if there is left sibling
	TreeNode* getLeftSibling(unsigned int numVotes); //provide sibling
	bool hasRightSibling(unsigned int numVotes);
	TreeNode* getRightSibling(unsigned int numVotes);
	bool hasSpare(); //check if there is at least 1 more than minimum numbder of keys
void transfer(TreeNode* rightSibling); //Transfer from current node to right sibling
void updateNonLeafKey(short offset,unsigned int minKey); //Propogation update
};

#endif /* TREENODE_H_ */

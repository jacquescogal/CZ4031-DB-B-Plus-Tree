/*
 * bptree.cpp
 *
 *  Created on: 3 Mar 2023
 *      Author: jacqu
 */

#include <stack>
#include "bptree.h"
#include "treenode.h"
#include <iostream>
#include <queue>
using namespace std;
BPTree::BPTree(short maxNodeKey) {
	// TODO Auto-generated constructor stub
	this->maxNodeKey=maxNodeKey;
	root=new TreeNode(maxNodeKey,true,0);
	nodeCount=1;
}

BPTree::~BPTree() {
	// TODO Auto-generated destructor st
}

void BPTree::insertRecord(unsigned int numVotes,Address address){
	// Types of insert:
	// Insert to existing key in leaf node
	// Insert to non-full leaf node
	// Insert to full leaf node (split insert)

	// Insert to non-leaf node
	// Insert to full non-leaf node (split insert)
	Block* block=(Block*)address.blockPtr;
		Record* record=(Record*)block->getRecord(address.offset);



	nPtr=root;
	stack<TreeNode*> nodeStack;
	while (!nPtr->isLeaf()){
		nodeStack.push(nPtr);
		nPtr=nPtr->goNext(numVotes);
	}
	if (!nPtr->isFull() || nPtr->hasKey(numVotes)){
		nPtr->insertRecord(numVotes,address);
	}
	else{
		while (nPtr->isFull()){
			TreeNode* newSibling = new TreeNode(maxNodeKey,nPtr->isLeaf(),nPtr->getHeight());
			nPtr->splitInsert(numVotes,newSibling,address);
			numVotes=newSibling->getMinKey();
			address={newSibling,0};
			++nodeCount;
			if (nodeStack.empty()){ //if nodestack is empty, then create new parent
				TreeNode* newParent= new TreeNode(maxNodeKey,false,nPtr->getHeight()+1);
				newParent->insertRecord(nPtr->getMinKey(),{nPtr,0},newSibling->getMinKey(),address);
				root=newParent;
				++nodeCount;
			}
			else{
				nPtr=nodeStack.top();
				nodeStack.pop();
				if (nPtr->isFull()) continue;
				else {
					nPtr->insertRecord(numVotes, address);
					break;
				}
			}
		}
	}
}


void BPTree::deleteRecord(unsigned int numVotes,Disk* disk){
	TreeNode* treePtr=root;
	stack<TreeNode*> nodeStack;
	stack<short> offsetStack;
	while (!treePtr->isLeaf()){
		nodeStack.push(treePtr);
		treePtr=treePtr->goNext(numVotes,offsetStack);
	}
	treePtr->deleteRecord(numVotes,disk);
	if (treePtr->isUnderflow(nodeStack.empty())){
		//case 1: borrow from sibling node(same parent)
		//case 2: merge if neither sibling has space
		TreeNode* parent=nodeStack.top();
		nodeStack.pop();
		short treePtrOffset=offsetStack.top();
		offsetStack.pop();
		bool borrowed=false;
		if (parent->hasLeftSibling(numVotes)){
			TreeNode* leftSibling=parent->getLeftSibling(numVotes);
			if (leftSibling->hasSpare()){
				leftSibling->transfer(treePtr);
				borrowed=true;
				parent->updateNonLeafKey(treePtrOffset,treePtr->getMinKey());
				while (!nodeStack.empty()){ //propogate changes
					treePtr=parent;
					TreeNode* parent=nodeStack.top();
					nodeStack.pop();
					short treePtrOffset=offsetStack.top();
					offsetStack.pop();
					parent->updateNonLeafKey(treePtrOffset,treePtr->getMinKey());
				}
				return;
			}
		}
		if (!borrowed && parent->hasRightSibling(numVotes)){
			TreeNode* rightSibling=parent->getLeftSibling(numVotes);
			if (rightSibling->hasSpare()){
				rightSibling->transfer(treePtr);
				treePtr=rightSibling;
				borrowed=true;
				parent->updateNonLeafKey(treePtrOffset+1,treePtr->getMinKey());
				while (!nodeStack.empty()){ //propogate changes
					treePtr=parent;
					TreeNode* parent=nodeStack.top();
					nodeStack.pop();
					short treePtrOffset=offsetStack.top();
					offsetStack.pop();
					parent->updateNonLeafKey(treePtrOffset,treePtr->getMinKey());
				}
				return;
			}
		}
	}
}

void BPTree::printRoot(){
	TreeNode* aPtr;
	short cHeight=-1;
	queue<string> stringQ;
	queue<TreeNode*> printQ;
	printQ.push(root);
	while (!printQ.empty()){
		aPtr=printQ.front();
		printQ.pop();
		if (cHeight!=aPtr->getHeight()){
			cHeight=aPtr->getHeight();
			stringQ.push("Level:"+to_string(cHeight)+"(root)");
		}
		stringQ.push(aPtr->getStringKeys());
	}
	while(!stringQ.empty()){
		string print=stringQ.front();
		stringQ.pop();
		cout<<print<<endl;
	}
}

void BPTree::printAll(){
	TreeNode* aPtr;
	short cHeight=-1;
	queue<string> stringQ;
	queue<TreeNode*> printQ;
	printQ.push(root);
	while (!printQ.empty()){
		aPtr=printQ.front();
		printQ.pop();
		if (cHeight!=aPtr->getHeight()){
			cHeight=aPtr->getHeight();
			stringQ.push("Level:"+to_string(cHeight));
		}
		stringQ.push(aPtr->getStringKeys());
		for (auto i:aPtr->getAllAddress()){
			printQ.push(i);
		}
	}
	while(!stringQ.empty()){
		string print=stringQ.front();
		stringQ.pop();
		cout<<print<<endl;
	}
}

unsigned int BPTree::getNodeCount(){
	return nodeCount;
}

std::vector<Record*> BPTree::getRecords(unsigned int numVotes,size_t* indexNodesAccessed,size_t* dataBlocksAccessed){
	nPtr=root;
	++(*indexNodesAccessed);
	while (!nPtr->isLeaf()){
		nPtr=nPtr->goNext(numVotes);
		++(*indexNodesAccessed);
	}
	return nPtr->getRecordsOfKey(numVotes,indexNodesAccessed,dataBlocksAccessed);
}

std::vector<Record*> BPTree::getRecordsInRange(unsigned int numVotesLower,unsigned int numVotesUpper,size_t* indexNodesAccessed,size_t* dataBlocksAccessed){
	nPtr=root;
	++(*indexNodesAccessed);
	std::vector<Record*> returnVector;
	set<Block*> blockPtrSet;
	while (!nPtr->isLeaf()){
		nPtr=nPtr->goNext(numVotesLower);
		++(*indexNodesAccessed);
	}
	nPtr->storeRecordsOfKeyUpToLimit(returnVector,blockPtrSet,numVotesLower,numVotesUpper,indexNodesAccessed);
	*dataBlocksAccessed=blockPtrSet.size();
	return returnVector;
}

short BPTree::getLevelCount(){
	return root->getHeight()+1;
}




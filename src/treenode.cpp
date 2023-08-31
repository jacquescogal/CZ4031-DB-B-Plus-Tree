/*
 * treenode.cpp
 *
 *  Created on: 3 Mar 2023
 *      Author: jacqu
 */

#include "treenode.h"
#include <iostream>
#include <math.h>
using namespace std;

TreeNode::TreeNode(short maxKey,bool isLeaf,short height) {
	this->maxKey=maxKey;
	this->maxPtr=maxKey+1;
	this->keyPtr=(unsigned int*) malloc(maxKey*sizeof(unsigned int));
	this->aPtr=(Address*) malloc(maxPtr*sizeof(Address));
	this->keyNum=0;
	this->ptrNum=0;
	this->l=isLeaf;
	this->h=height;
	this->neighbourPtr=false;
}

TreeNode::TreeNode(short maxPtr) { //For Linked List
	this->maxPtr=maxPtr;
	this->aPtr=(Address*) malloc(maxPtr*sizeof(Address));
	this->ptrNum=0;
	this->neighbourPtr=false;
}

TreeNode::~TreeNode() {
	// TODO Auto-generated destructor stub
}

bool TreeNode::isFull(){
	if (keyNum<maxKey)return false;
	else return true;
}

bool TreeNode::isLeaf(){
	return l;
}

void TreeNode::llSetNeighbour(TreeNode* listNode){
	neighbourPtr=true;
	*(aPtr+maxPtr-1)={listNode,0};
}

TreeNode* TreeNode::llInsertRecord(Address address){
	if (!llIsFull()){
		*(aPtr+ptrNum)=address;
		++ptrNum;
		return this;
	}
	else{
		TreeNode* LLNewCurrent=new TreeNode(floor(200/sizeof(Address)));
		LLNewCurrent->llInsertRecord(address);
		LLNewCurrent->llSetNeighbour(this);
		return LLNewCurrent;
	}
}

bool TreeNode::llIsFull(){
	if (ptrNum>=maxPtr-1) return true;
	else return false;
}

void TreeNode::llPrint(){
	for (short i=0;i<ptrNum;++i){
		cout<<(aPtr+i)->offset<<'|';
	}
	cout<<endl;
}

void TreeNode::insertRecord(unsigned int numVotes, Address address, bool copy){
	if (copy){
		if (l){
			short i;
			for (i=0;i<maxKey && i<keyNum;++i){
				if (*(keyPtr+i)==numVotes){
					TreeNode* LLNode=(TreeNode*)(aPtr+i)->blockPtr;
					LLNode=LLNode->llInsertRecord(address);
					*(aPtr+i)={LLNode,0};
					return;
				}
				else if (*(keyPtr+i)>numVotes)break;
			}
			for (short j=keyNum;j>i;--j){
				*(keyPtr+j)=*(keyPtr+j-1);
				*(aPtr+j)=*(aPtr+j-1);
			}
			*(keyPtr+i)=numVotes;
			*(aPtr+i)=address;
			++keyNum;
	}
		else{
				short i;
				for (i=0;i<maxKey && i<keyNum;++i){
					if (*(keyPtr+i)>numVotes)break;
				}
				if (i==keyNum){
					*(keyPtr+i)=numVotes;
					*(aPtr+i+1)=address;
					++keyNum;
					++ptrNum;
				}
				else if (i==0 && minKey>numVotes){
					*(aPtr+keyNum+1)=*(aPtr+keyNum);
					for (short j=keyNum;j>i;--j){
						*(keyPtr+j)=*(keyPtr+j-1);
						*(aPtr+j)=*(aPtr+j-1);
					}
					*(aPtr)=address;
					*(keyPtr)=minKey;
					minKey=numVotes;
					++keyNum;
					++ptrNum;
				}
				else {
					for (short j=keyNum;j>i;--j){
						*(keyPtr+j)=*(keyPtr+j-1);
						*(aPtr+j+1)=*(aPtr+j);
					}
					*(aPtr+i+1)=address;
					*(keyPtr+i)=numVotes;
					++keyNum;
					++ptrNum;
				}

			}
	}
}

void TreeNode::insertRecord(unsigned int numVotes, Address address){
	if (l){
	short i;
	for (i=0;i<maxKey && i<keyNum;++i){
		if (*(keyPtr+i)==numVotes){
			TreeNode* LLNode=(TreeNode*)(aPtr+i)->blockPtr;
			LLNode=LLNode->llInsertRecord(address);
			*(aPtr+i)={LLNode,0};
			return;
		}
		else if (*(keyPtr+i)>numVotes)break;
	}
	for (short j=keyNum;j>i;--j){
		*(keyPtr+j)=*(keyPtr+j-1);
		*(aPtr+j)=*(aPtr+j-1);
	}

	TreeNode* LLNode=new TreeNode(floor(200/sizeof(Address))); //LLNode only consists of addresses
	Block* block=(Block*)address.blockPtr;
	Record* record=(Record*)block->getRecord(address.offset);
	LLNode->llInsertRecord(address);
	*(keyPtr+i)=numVotes;
	*(aPtr+i)={LLNode,0};
	++keyNum;
	}
	else{
		short i;
		for (i=0;i<maxKey && i<keyNum;++i){
			if (*(keyPtr+i)>numVotes)break;
		}
		if (i==keyNum){
			*(keyPtr+i)=numVotes;
			*(aPtr+i+1)=address;
			++keyNum;
			++ptrNum;
		}
		else if (i==0 && minKey>numVotes){
			*(aPtr+keyNum+1)=*(aPtr+keyNum);
			for (short j=keyNum;j>i;--j){
				*(keyPtr+j)=*(keyPtr+j-1);
				*(aPtr+j)=*(aPtr+j-1);
			}
			*(aPtr)=address;
			*(keyPtr)=minKey;
			minKey=numVotes;
			++keyNum;
			++ptrNum;
		}
		else {
			for (short j=keyNum;j>i;--j){
				*(keyPtr+j)=*(keyPtr+j-1);
				*(aPtr+j+1)=*(aPtr+j);
			}
			*(aPtr+i+1)=address;
			*(keyPtr+i)=numVotes;
			++keyNum;
			++ptrNum;
		}

	}
}
void TreeNode::insertRecord(unsigned int numVotesLeft, Address addressLeft,unsigned int numVotesRight,Address addressRight){
	*(aPtr)=addressLeft;
	*(aPtr+1)=addressRight;
	ptrNum+=2;
	*(keyPtr)=numVotesRight;
	++keyNum;
	minKey=numVotesLeft;
}

void TreeNode::printAll(){
	short i;
	for (i=0;i<keyNum;++i){
		cout<<i<<'|'<<"pointer"<<(*(aPtr+i)).blockPtr<<'|'<<*(keyPtr+i)<<endl;
	}
	if (!l) cout<<i<<'|'<<"pointer"<<(*(aPtr+i)).blockPtr<<endl;
}

void TreeNode::setNeighbour(TreeNode* neighbour){
	neighbourPtr=true;
	*(aPtr+maxPtr-1)={neighbour,0};
}

void TreeNode::splitInsert(unsigned int numVotes,TreeNode* siblingNode,Address address){
	if (l){ //leafNode
		short i;
		bool addCurrent=true;
		keyNum=floor((maxKey+1)/2)-1;
		if (*(keyPtr+keyNum)<numVotes){
			++keyNum;
			addCurrent=false;
		}
		for (i=keyNum;i<maxKey;++i){
			siblingNode->insertRecord(*(keyPtr+i),*(aPtr+i),true);
		}
		if (neighbourPtr==true){
			TreeNode* oldNeighbour=(TreeNode*)(aPtr+maxPtr-1)->blockPtr;
			siblingNode->setNeighbour(oldNeighbour);
		}
		setNeighbour(siblingNode);
		if (addCurrent) insertRecord(numVotes,address);
		else siblingNode->insertRecord(numVotes,address);
	}
	else{ //non-leafNode
		short minChild=floor(maxKey/2)+1;
		short minKeyNum=floor(maxKey/2);
		vector<Address> addressVector;
		vector<unsigned int> keyVector;
		short i;
		addressVector.push_back(aPtr[0]);
		keyVector.push_back(minKey);
		bool sentinal=false;
		for (i=0;i<maxKey && i<keyNum;++i){
			if (*(keyPtr+i)>numVotes && sentinal==false){
				addressVector.push_back(address);
				keyVector.push_back(numVotes);
				sentinal=true;
			}
			addressVector.push_back(aPtr[i+1]);
			keyVector.push_back(keyPtr[i]);
		}
		if (sentinal==false){
			addressVector.push_back(address);
			keyVector.push_back(numVotes);
			sentinal=true;
		}
		keyNum=ceil(maxKey/2)+1;
		aPtr[0]=addressVector[0];
		minKey=keyVector[0];
		for (short i=0;i<keyNum;++i){
			aPtr[i+1]=addressVector[i+1];
			keyPtr[i]=keyVector[i+1];
		}
		siblingNode->insertRecord(keyVector[keyNum+1],addressVector[keyNum+1],keyVector[keyNum+2],addressVector[keyNum+2]);
		for (short i=keyNum+3;i<keyVector.size();++i){
			siblingNode->insertRecord(keyVector[i],addressVector[i]);
		}
	}
}

unsigned int TreeNode::getMinKey(){
	if (l)return *keyPtr;
	else return minKey;
}

TreeNode* TreeNode::goNext(unsigned int numVotes){
	if (l){
		cout<<"error, meant to be non-leaf"<<endl;
		return NULL;
	}
	else{
		short i;
		for (i=0;i<keyNum;++i){
			if (numVotes<*(keyPtr+i)){
				return (TreeNode*)(*(aPtr+i)).blockPtr;
			}
		}
		return (TreeNode*)(*(aPtr+i)).blockPtr;
	}
}
TreeNode* TreeNode::goNext(unsigned int numVotes,stack<short> &offsetStack){
	if (l){
		cout<<"error, meant to be non-leaf"<<endl;
		return NULL;
	}
	else{
		short i;
		for (i=0;i<keyNum;++i){
			if (numVotes<*(keyPtr+i)){
				offsetStack.push(i);
				return (TreeNode*)(*(aPtr+i)).blockPtr;
			}
		}
		offsetStack.push(i);
		return (TreeNode*)(*(aPtr+i)).blockPtr;
	}
}


string TreeNode::getStringKeys(){
	string rString;
	rString.push_back('|');
	for (short i=0;i<keyNum;++i){
		rString+=to_string(*(keyPtr+i));
		rString+='|';
	}
	for (short i=keyNum;i<maxKey;++i){
		rString+='-';
		rString+='|';
	}
	return rString;
}

short TreeNode::getHeight(){
	return h;
}

vector<TreeNode*> TreeNode::getAllAddress(){
	vector<TreeNode*> returnVector;
	if (!l){
		short j;
		for (j=0;j<keyNum;++j){
			returnVector.push_back((TreeNode*)(*(aPtr+j)).blockPtr);
		}
		returnVector.push_back((TreeNode*)(*(aPtr+j)).blockPtr);
	}
	return returnVector;
}

void TreeNode::deleteRecord(unsigned int numVotes,Disk* disk){
	short i;
	for (i=0;i<keyNum;++i){
		if (keyPtr[i]==numVotes){
			TreeNode* LLNode=(TreeNode*)(aPtr[i]).blockPtr;
			LLNode->llDeleteChain(disk);
			--keyNum;
		}
	}
}

void TreeNode::llDeleteChain(Disk* disk){
	for (short i=0;i<ptrNum;++i){
		disk->deleteRecord(*(aPtr+i));
	}
	if (neighbourPtr)((TreeNode*)(aPtr[maxPtr-1]).blockPtr)->llDeleteChain(disk);
	free(aPtr);
	delete(this);
}

void TreeNode::llRetrieveChain(std::vector<Record*> &vectorRecord,size_t* indexNodesAccessed,set<Block*> &blkPtrSet){
	++(*indexNodesAccessed);
	for (short i=0;i<ptrNum;++i){
		Block* block=(Block*)(aPtr+i)->blockPtr;
		blkPtrSet.insert(block);
		vectorRecord.push_back(block->getRecord((aPtr+i)->offset));
	}
	if (neighbourPtr)((TreeNode*)(aPtr[maxPtr-1]).blockPtr)->llRetrieveChain(vectorRecord,indexNodesAccessed,blkPtrSet);
}

std::vector<Record*> TreeNode::getRecordsOfKey(unsigned int numVotes,size_t* indexNodesAccessed,size_t* dataBlocksAccessed){
	vector<Record*> vectorRecord;
	set<Block*> blockPtrSet;
	for(short i=0;i<keyNum;++i){
		if (keyPtr[i]==numVotes){
			TreeNode* LLNode=(TreeNode*)(aPtr+i)->blockPtr;
			LLNode->llRetrieveChain(vectorRecord,indexNodesAccessed,blockPtrSet);
		}
}
	*dataBlocksAccessed=(size_t)blockPtrSet.size();
	return vectorRecord;
}

bool TreeNode::hasKey(unsigned int numVotes){
	for (short i=0;i<keyNum;++i){
		if (*(keyPtr+i)==numVotes){
			return true;
		}
	}
	return false;
}

void TreeNode::storeRecordsOfKeyUpToLimit(std::vector<Record*> &recordsVector,set<Block*> &blockPtrSet,unsigned int numVotesLower,unsigned int numVotesUpper,size_t* indexNodesAccessed){
	bool toContinue=false;
	for(short i=0;i<keyNum;i++){
		if (keyPtr[i]>=numVotesLower && keyPtr[i]<=numVotesUpper){
			TreeNode* LLNode=(TreeNode*)(aPtr+i)->blockPtr;
			LLNode->llRetrieveChain(recordsVector,indexNodesAccessed,blockPtrSet);
			toContinue=true;
		}
		else if (keyPtr[i]>numVotesUpper){
			toContinue=false;
		}
}
	if (toContinue && neighbourPtr){
		TreeNode* neighbour=(TreeNode*)(aPtr+maxPtr-1)->blockPtr;
		++(*indexNodesAccessed);
		neighbour->storeRecordsOfKeyUpToLimit(recordsVector,blockPtrSet,numVotesLower,numVotesUpper,indexNodesAccessed);
	}
}

bool TreeNode::isUnderflow(bool root){
	if (root)return false;
	else if (l)return (floor((maxKey+1)/2)>keyNum); //except root
	else return (floor(maxKey/2)>keyNum);
}

bool TreeNode::hasLeftSibling(unsigned int numVotes){
	if (l){
			cout<<"error, meant to be non-leaf"<<endl;
			return NULL;
		}
		else{
			short i;
			for (i=0;i<keyNum;++i){
				if (numVotes<*(keyPtr+i)){
					if (i==0)return false;
					else return true;
				}
			}
			if (i==0)return false;
			else return true;
		}
}

bool TreeNode::hasRightSibling(unsigned int numVotes){
	if (l){
			cout<<"error, meant to be non-leaf"<<endl;
			return NULL;
		}
		else{
			short i;
			for (i=0;i<keyNum;++i){
				if (numVotes<*(keyPtr+i)){
					return true;
				}
			}
			return false;
		}
}

TreeNode* TreeNode::getLeftSibling(unsigned int numVotes){
	if (l){
		cout<<"error, meant to be non-leaf"<<endl;
		return NULL;
	}
	else{
		short i;
		for (i=0;i<keyNum;++i){
			if (numVotes<*(keyPtr+i)){
				return (TreeNode*)(*(aPtr+i-1)).blockPtr;
			}
		}
		return (TreeNode*)(*(aPtr+i-1)).blockPtr;
	}
}

TreeNode* TreeNode::getRightSibling(unsigned int numVotes){
	if (l){
		cout<<"error, meant to be non-leaf"<<endl;
		return NULL;
	}
	else{
		short i;
		for (i=0;i<keyNum;++i){
			if (numVotes<*(keyPtr+i)){
				return (TreeNode*)(*(aPtr+i+1)).blockPtr;
			}
		}
		return (TreeNode*)(*(aPtr+i+1)).blockPtr;
	}
}

bool TreeNode::hasSpare(){
	// consider how non non-leafs and their pointers interact with this
	if (l)return (floor((maxKey+1)/2)<keyNum);
	else return (floor(maxKey/2)<keyNum);
}

void TreeNode::transfer(TreeNode* rightSibling){
	unsigned int toTransfer=*(keyPtr+keyNum-1);
	Address toTransferAdd;
	if (l) toTransferAdd=*(aPtr+keyNum-1);
	else toTransferAdd=*(aPtr+keyNum);
	--keyNum;
	rightSibling->insertRecord(toTransfer,toTransferAdd,true);
}


void TreeNode::updateNonLeafKey(short offset,unsigned int minKey){
	//0 isminKey
	if (offset==0){
		this->minKey=minKey;
	}
	else{
		*(keyPtr+offset-1)=minKey;
	}
}

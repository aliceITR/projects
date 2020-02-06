/*
 * partionmanager.cpp
 * Function definitions defined in the partionmanager.h file
 */

 /*
  * Modified By S Blair
  * Date: Nov 11, 2019
  * What: Implemented Functions in the Partiotion manager
  */

/*
Modified by Gage Brown
11.11.2019
Set up the partition's bitvector block
Write blank char array to deallocated blocks
GB: 11.19.2019
BitVector block is now set as allocated
*/

/*
Modified by S Blair
11.27.2019
Finished partiotion manager
there might be compiler errors
*/

#include "partitionmanager.h"
#include "bitvector.h"
#include <iostream>
using namespace std;


PartitionManager::PartitionManager(DiskManager* dm, char partitionname, int partitionsize)
{
	myDM = dm;
	myPartitionName = partitionname;
	myPartitionSize = partitionsize;

	//SB: Created a BitVector to have the entire partition data in it
	//need to create a root directory
	myBitVector = new BitVector(myPartitionSize);

	myBitVector->setBit(0); //GB: sets the bitvector block as allocated

	myBitVector->setBit(1); // set bock 1 as not free (for root directory)

	//GB: for now I'm assuming that directory i-nodes are stored directly in the block of the directory itself

	char root[64];
	readDiskBlock(1, root);
	if (root[1] == '#')
	{
		for (int i = 0; i < 64; i++) root[i] = '#';
		int r = this->writeDiskBlock(1, root);
	}
	else
	{
		char bitvec[64];
		for (int i = 0; i < 64; i++) bitvec[i] = '.';
		readDiskBlock(0, bitvec);
		myBitVector->setBitVector((unsigned int*)bitvec);
	}

	//GB: initialize the partitions bitvector block
	updateBitVectorBlock();
}

//Default Constructor
PartitionManager::PartitionManager()
{
	//Does nothing but allow me to create an array of partition managers as a member of the filesystem
}


//Deconstructor
PartitionManager::~PartitionManager() 
{
	delete myBitVector;
	myPartitionName = ' ';
	myPartitionSize = 0;
}


/*
 * SB: Allocates the first free block and returns that position
 * returns blocknum
 * or -1 if the partition is full
 */
int PartitionManager::getFreeDiskBlock()
{
	
	for (int i = 0; i < myPartitionSize; i++)
	{
		if (myBitVector->testBit(i) == 0) //SB: test if the block is available
		{
            char buffer[64];
			for (int k = 0; k < 64; k++) buffer[k] = '#';
			
			myBitVector->setBit(i); //SB: Set that bit as used
			updateBitVectorBlock();

            writeDiskBlock(i, buffer);
			
			return i; //SB: returns the position of the set bit
		}
	}
	return -1; // SB: returns -1 if the partition s full
    
}


/*
 * SB: Deallocates the block given a position named blknum
 * Returns 0 if it was sucessful
 * Returns -1 in any other case
 */
int PartitionManager::returnDiskBlock(int blknum)
{
	if (myBitVector->testBit(blknum) == 1)
	{
		myBitVector->resetBit(blknum);
		updateBitVectorBlock();

		char emptyBuffer[getBlockSize()];
		for (int i = 0; i < getBlockSize(); i++) emptyBuffer[i] = '#';

		//GB: write the blank char array into the block or return -1 if writing fails
		return (writeDiskBlock(blknum, emptyBuffer)) ? -1 : 0;
	}
	return -1;
}

/*
returns:
	0: if the block is successfully read;
   -1: if disk can't be opened; (same as disk)
   -2: if blknum is out of bounds; (same as disk)
   -3: if partition doesn't exist
 */
int PartitionManager::readDiskBlock(int blknum, char* blkdata)
{
	return myDM->readDiskBlock(myPartitionName, blknum, blkdata);
}

/*
returns:
	0: if the block is successfully written;
   -1: if disk can't be opened; (same as disk)
   -2: if blknum is out of bounds;  (same as disk)
   -3: if partition doesn't exist
 */
int PartitionManager::writeDiskBlock(int blknum, char* blkdata)
{
	if (blknum == 0) return -2; //GB: no writing directly to the bitvector
	return myDM->writeDiskBlock(myPartitionName, blknum, blkdata);
}

int PartitionManager::getBlockSize()
{
	return myDM->getBlockSize();
}

/*
Doesn't make any changes to the bitVector, those are handled in the partitionManager functions
that deal with file allocation. The purpose of this function is to take the changes already
made to the bitvector and update them in block 0 of the partition
*/
void PartitionManager::updateBitVectorBlock()
{
	char buffer[myPartitionSize]; //GB: creates an empty buffer for the bitvector to be read into
	for (int i = 0; i < myPartitionSize; i++) buffer[i] = '.';

	//GB: reads the bitvector into buffer
	myBitVector->getBitVector((unsigned int*)buffer);

	//GB: writes buffer to the bitVectorBlock
	myDM->writeDiskBlock(myPartitionName, 0, buffer);
}

/*
diskmanager.cpp
Edited By: Gage Brown
11.06.2019

GB: 11.19.2019
Set up the super block
*/

/*
Edited By: S. Blair
Nov 11, 2019
What: Fixed A compiler error of redefinition

Edited By S. Blair
Nov 18, 2019
what: Updated the Constructor and got rid of the infinite while loops
and used for loops instead
*/

#include "diskmanager.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

#define blockSize 64

DiskManager::DiskManager(Disk* d, int partcount, DiskPartition* dp)
{
	myDisk = d;
	partCount = partcount;

	int r = myDisk->initDisk();

	if (r == 1)
	{
		diskP = dp;
		createSuperBlock();
	}
	else //The disk has already been created
	{
		char buffer[blockSize];
		myDisk->readDiskBlock(0, buffer);
		if (buffer[0] == '#')//GB: the super block is empty
		{
			diskP = dp;
			createSuperBlock();
		}
		else //GB: read partition information from the super block
		{
			int parts = charToInt(0, buffer);
			diskP = new DiskPartition[parts];
			int position = 4; 
			for (int i = 0; i < parts; i++)
			{
				diskP[i].partitionName = buffer[position++];
				diskP[i].partitionSize = charToInt(position, buffer);
				position += 4;
			}
		}
	}
}

DiskManager::~DiskManager()
{
	partCount = 0;

}

/*
SuperBlock:
	1: first 4 bytes: the number of partitions
	2: 5th byte: first partition name
	3: 6th-9th bytes: partition size
2-3 repeat for each partition

After partition info:
	pointers to i-nodes?
*/
void DiskManager::createSuperBlock()
{
	char buffer[blockSize];
	for (int i = 0; i < blockSize; i++) buffer[i] = '#';
	int position = 0;
	int pStartingBlock = 1;

	intToChar(0, partCount, buffer);
	position += 4;
	for (int i = 0; i < partCount; i++)
	{
		buffer[position++] = diskP[i].partitionName;
		intToChar(position, diskP[i].partitionSize, buffer);
		position += 4;
	}
	myDisk->writeDiskBlock(0, buffer);
}

void DiskManager::readSuperBlock(char* buffer)
{
	myDisk->readDiskBlock(0, buffer);
}

/*
 GB: Written under the assumtion that blknum refers to a block within the
	 partition and that an offset is required in order to access the correct
	 block within the disk. I'm also operating under the assumtions that (1)
	 partitions are ordered in the disk in the same order in which they are
	 initialized and (2) once we reach the last block in one partition the next
	 block is the start of the next partition.

	 if these assumptions don't hold, the DiskManager read/write functions do not work.
*/
/*
returns:
	0: if the block is successfully read;
   -1: if disk can't be opened; (same as disk)
   -2: if blknum is out of bounds; (same as disk)
   -3: if partition doesn't exist
 */
int DiskManager::readDiskBlock(char partitionname, int blknum, char* blkdata)
{
	int pSize = getPartitionSize(partitionname);
	if (pSize == -1)
	{
		return -3;
	}
	else if (pSize < blknum)
	{
		return -2; //GB: blknum is outside of the partition
	}
	int offset = getDiskOffset(partitionname);
	return myDisk->readDiskBlock(blknum + offset, blkdata);
}


/*
returns:
	0: if the block is successfully written;
   -1: if disk can't be opened; (same as disk)
   -2: if blknum is out of bounds;  (same as disk)
   -3: if partition doesn't exist
 */
int DiskManager::writeDiskBlock(char partitionname, int blknum, char* blkdata)
{
	/* write the code for writing a disk block to a partition */
	int pSize = getPartitionSize(partitionname);
	if (pSize == -1)
	{
		return -3;
	}
	else if (pSize < blknum)
	{
		return -2; //GB: blknum is outside of the partition
	}
	int offset = getDiskOffset(partitionname);
	return myDisk->writeDiskBlock(blknum + offset, blkdata);
}

/*
 * return size of partition
 * -1 if partition doesn't exist.
 */
int DiskManager::getPartitionSize(char pName)
{
	int pSize = -1;
	/* write the code for returning partition size */
		//SB: For some reason it would never exit the while loop so i changed it to a for loop
	for (int i = 0; i < partCount; i++)
	{
		if (diskP[i].partitionName == pName)
		{
			pSize = diskP[i].partitionSize;
			break;
		}
	}
	return pSize;
}

/*
GB: returns an offset which is added to the partition blknum in order to
	access the corresponding disk blknum when we call the disk read/write
	functions from the diskmanager read/write functions
*/
int DiskManager::getDiskOffset(char pName)
{
	int offset = 1; //GB: set to one to account for the SuperBlock
	bool targetPartitionReached = false;
	
	int i = 0;
	while (i < partCount && !targetPartitionReached)
	{
		if (pName == diskP[i].partitionName)
		{
			targetPartitionReached = true;
		}
		else
		{
			offset += diskP[i++].partitionSize;
		}
	}


	return offset;
}

/*
 * takes a position and buffer and a number.
 * and will convert the number to a 4 characters and starting at the position.
 *   if the number is shorter then 4 digits, use zeros to fill left positions.
 *
 *  example:  buff= "1234567890"
 *  pos=3, num=0  so buff is changed to "1230000890"
 *     0 is converted to 0000
 *  pos=1, num=102 so buff is  "1010200890"
 *         Note, 102 was converted to 0102  Don't forget the 0
*/

void DiskManager::intToChar(int pos, int num, char* buff)
{

	const int numDigits = 4;
	int leadingZeros;

	if (num < 10) leadingZeros = 3;
	else if (num < 100) leadingZeros = 2;
	else if (num < 1000) leadingZeros = 1;
	else if (num < 10000) leadingZeros = 0;
	else
	{
		cerr << "Invalid input \n";
		return;
	}

	int count = 0;
	while (count < leadingZeros)
	{
		buff[pos++] = '0';
		++count;
	}
	
	pos += (numDigits - leadingZeros) - 1;
	for (int i = 0; i < (numDigits - leadingZeros); i++)
	{
		int temp = num % 10;
		num /= 10;
		buff[pos--] = '0' + temp;
	}

	return;
}

int DiskManager::charToInt(int pos, char* buff)
{
	char toInt[5];
	for (int i = 0; i < 4; i++)
	{
		toInt[i] = buff[pos++];
	}
	toInt[4] = '\0';
	return atoi(toInt);
}
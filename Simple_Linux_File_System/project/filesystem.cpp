/*
* filesystem.cpp
* CPP file for the filesystem
*/

/*
* Modiefied By S Blair
* Date: Nov 18, 2019
* What: Made constructor and part of create file
*/

/*
* Modiefied By S Blair
* Date: Nov 27, 2019
* What: Added a check if it is a valid filename
*/

#include "partitionmanager.h"
#include "filesystem.h"
#include <time.h>
#include <cstdlib>

#include <string.h>
#include <iostream>

using namespace std;

FileSystem::FileSystem(DiskManager* dm, char fileSystemName)
{
	myDM = dm;
	this->fileSystemName = fileSystemName;
	fileDesc = 0;
	lockIDCounter = 1;

	//GB: creating partition manager
	char partInfo[64];
	myDM->readSuperBlock(partInfo); //GB: reads superBlock info into partInfo

	int pos = 0;
	int numParts = myDM->charToInt(0, partInfo);
	pos += 4;

	fileSystemSize = 0;

	for (int i = 0; i < numParts; i++)
	{
		char pName = partInfo[pos++];
		if (pName == fileSystemName)
		{
			fileSystemSize = myDM->charToInt(pos, partInfo);
			pos += 4;
			myPM = new PartitionManager(dm, pName, fileSystemSize);
			break;
		}
		else
		{
			pos += 4;
		}
	}
}

/*
Creates a new file whose name is pointed to by filename of size fname_len characters.
File names and directory names start with `/' character and consist of a sequence of alternating
`/' and alphabet (`A' to `Z' and `a' to `z') characters ending with an alphabet character.
returns: 0 if the file is created successfully.
		-1 if the file already exists
		-2 if there is not enough disk space - done
		-3 if invalid filename - done
		-4 if the file cannot be created for some other reason
*/
int FileSystem::createFile(char* filename, int fnameLen)
{
	if (!isValidName(filename, fnameLen)) return -3;
	//GB: the path for 'filename' already exists
	if (pathExists(filename, fnameLen))
	{
		char pebuffer[64];
		int check = getInode(filename, fnameLen, 'f', pebuffer);
		if (check == -2) return -4;
		else return -1;
	}

	char dirInode[64];
	int dirBlkNum = 1;
	if (fnameLen > 2)
	{
		char dirPath[fnameLen - 2];
		for (int i = 0; i < (fnameLen - 2); i++) dirPath[i] = filename[i];
		dirBlkNum = getInode(dirPath, (fnameLen - 2), 'd', dirInode);
		if (dirBlkNum < 0) return -4; //GB: directory doesn't exist
	}
	myPM->readDiskBlock(dirBlkNum, dirInode); //GB: root directory

	int blknum = myPM->getFreeDiskBlock(); //GB: for file inode
	if (blknum == -1) return -2; //GB: no space within the partition
	else
	{
		int inodeBlknum = blknum;

		/* -------------------------------- Directory Inode -------------------------------- */

		int pos;
		bool found = false, done = false;
		while (!done)
		{
			pos = 0;
			for (pos; pos < 60; pos += 6)
			{
				if (dirInode[pos] == '#')
				{
					found = true;
					done = true;
					break;
				}
			}
			int temp = myDM->charToInt(60, dirInode);
			if (temp > 0 && done != true)
			{
				dirBlkNum = temp;
				myPM->readDiskBlock(temp, dirInode);
			}
			else done = true;
		}

		if (found == false)//SB: Could not find room
		{
			int newBlk = myPM->getFreeDiskBlock(); 
			if (newBlk == -1) return -2; //no space within the partition
			//gotta allocate another block for the directory
			int c = extendDirectory(inodeBlknum, newBlk);
			if (c < 0) return -4;
			char newDir[myPM->getBlockSize()];
			myPM->readDiskBlock(newBlk, newDir);
			pos = 6;
			for (pos; pos < 60; pos += 6) if (newDir[pos] == '#') break;

			newDir[pos++] = filename[fnameLen - 1];
			newDir[pos++] = 'f';
			myDM->intToChar(pos, inodeBlknum, newDir); //pointer to file inode
			myDM->intToChar(60, newBlk, dirInode); //pointer to file inode
			myPM->writeDiskBlock(dirBlkNum, dirInode);

			myPM->writeDiskBlock(newBlk, newDir);

		}
		else
		{
			dirInode[pos++] = filename[fnameLen - 1];
			dirInode[pos++] = 'f';
			myDM->intToChar(pos, inodeBlknum, dirInode); //pointer to file inode
			myPM->writeDiskBlock(dirBlkNum, dirInode);
		}

		/* ----------------------------------- File Inode ----------------------------------- */

		char fileInode[myPM->getBlockSize()];
		for (int i = 0; i < myPM->getBlockSize(); i++) fileInode[i] = '#';


		pos = 0;
		fileInode[pos++] = filename[fnameLen - 1];
		fileInode[pos++] = 'f';
		myDM->intToChar(pos, 0, fileInode);
		pos += 4;
		myDM->intToChar(pos, 0, fileInode); //direct block pointer 1
		pos += 4;
		myDM->intToChar(pos, 0, fileInode); //direct block pointer 2: empty
		pos += 4;
		myDM->intToChar(pos, 0, fileInode); //direct block pointer 3: empty
		pos += 4;
		myDM->intToChar(pos, 0, fileInode); //indirect block pointer: empty
		pos += 4;

		myPM->writeDiskBlock(inodeBlknum, fileInode);

		return 0;
	}
}

/*
Creates a new directory whose name is pointed to by dirname.
returns: 0 if the directory is created successfully.
		-1 if the directory already exists
		-2 if there is not enough disk space
		-3 if invalid directory name
		-4 if the directory cannot be created for some other reason
*/
int FileSystem::createDirectory(char* dirname, int dnameLen)
{
    
    ////////////////////////////////////////////CHECKS////////////////////////////////////////////
	if (!isValidName(dirname, dnameLen)) return -3;
	//GB: the path for 'filename' already exists
	if (pathExists(dirname, dnameLen))
	{
		char pebuffer[64];
		int check = getInode(dirname, dnameLen, 'd', pebuffer);
		if (check == -2) return -4;
		else return -1;
	}
        
        /////////////////////////////////Get The Head Directory///////////////////////////////////////////////////

	char dirInode[64];
	int dirBlkNum = 1;
	if (dnameLen > 2)
	{
		char dirPath[dnameLen - 2];
		for (int i = 0; i < (dnameLen - 2); i++) dirPath[i] = dirname[i];
		dirBlkNum = getInode(dirPath, (dnameLen - 2), 'd', dirInode);
		if (dirBlkNum < 0) return -3; 
	}
	
	myPM->readDiskBlock(dirBlkNum, dirInode); 
        //for (int i = 0; i < 64 ; i ++) cout << dirInode;
        //cout << endl;

	int blknum = myPM->getFreeDiskBlock();
	if (blknum == -1) return -2; //GB: no space within the partition
	else
	{
		int inodeBlknum = blknum;


		/* -------------------------------- Super Directory Inode -------------------------------- */

		int pos = 0;
		bool found = false, done = false;
		while (!done)
		{
			pos = 0;
			for (pos; pos < 60; pos += 6)
			{
				if (dirInode[pos] == '#')
				{
					found = true;
					done = true;
					break;
				}
			}
			int temp = myDM->charToInt(60, dirInode);
			if (temp > 0 && done != true)
			{

				dirBlkNum = temp;
				myPM->readDiskBlock(temp, dirInode);
			}
			else
			{
				done = true;
			}

		}
		if (found == false)//SB: Could not find room
		{

			int newBlk = myPM->getFreeDiskBlock();
			if (newBlk == -1) return -2; //no space within the partition
			//gotta allocate another block for the directory
			int c = extendDirectory(inodeBlknum, newBlk);
			if (c < 0)
			{
				return -4;
			}
			char newDir[myPM->getBlockSize()];
			myPM->readDiskBlock(newBlk, newDir);
			pos = 6;
			for (pos; pos < 60; pos += 6)
			{
				if (newDir[pos] == '#')
				{
					break;
				}
			}

			newDir[pos++] = dirname[dnameLen - 1];
			newDir[pos++] = 'd';
			myDM->intToChar(pos, inodeBlknum, newDir); //pointer to file inode
			myDM->intToChar(60, newBlk, dirInode); //pointer to file inode
			myPM->writeDiskBlock(dirBlkNum, dirInode);

			myPM->writeDiskBlock(newBlk, newDir);

		}
		else
		{
			dirInode[pos++] = dirname[dnameLen - 1];
			dirInode[pos++] = 'd';
			myDM->intToChar(pos, inodeBlknum, dirInode); //pointer to file inode
			myPM->writeDiskBlock(dirBlkNum, dirInode);
                                            
		}

		/* ----------------------------------- Directory Inode ----------------------------------- */

		char fileInode[myPM->getBlockSize()];
		for (int i = 0; i < myPM->getBlockSize(); i++) fileInode[i] = '#';

		myPM->writeDiskBlock(inodeBlknum, fileInode);
		return 0;
	}
}

/*
Locks a file.
A file cannot be locked if (1) it doesn't exist, or (2) it is already locked, or (3) it is currently opened.
A note, once a file is locked, it may only be opened with the lock id and the file cannot be deleted or renamed until the file is unlocked.
returns: A number greater than 0 (lock id) if the file is successfully locked
		-1 if the file is already locked
		-2 if the file does not exist
		-3 if it is currently opened
		-4 if the file cannot be locked for any other reason.
*/
int FileSystem::lockFile(char* filename, int fnameLen)
{
	if (!isValidName(filename, fnameLen))
	{
		return -4;//misc check
	}

	int lockID;
	if (pathExists(filename, fnameLen))
	{
		char type = '%';
		getBlockNumber(filename, fnameLen, type);
		if (type != 'f') return -4; //GB: not a file

		if (locked.find(filename) != locked.end() && locked[filename] != -1) return -1; //Already Locked
		if (isOpen(filename, fnameLen)) return -3; //Already open

		lockID = lockIDCounter++;

		locked[filename] = lockID;
		return lockID;
	}
	else
	{
		return -2; //File does not exist
	}
}

/*
Unlocks a file.
The lock_id is the lock id returned by the LockFile function when the file was locked.
returns: 0 if successful
		-1 if lock id is invalid
		-2 for any other reason.
*/
int FileSystem::unlockFile(char* filename, int fnameLen, int lockId)
{
	if (!isValidName(filename, fnameLen))
	{
		return -2;//misc check
	}
	int lockID;
	if (pathExists(filename, fnameLen))
	{
		if (locked.find(filename) == locked.end()) return -2; //Is not Locked
		if (locked[filename] != lockId) return -1; //Wrong ID

		locked.erase(filename);
		return 0;
	}
	else
	{
		return -2; //File does not exist
	}
}

/*
Deletes the file whose name is pointed to by filename.
A file that is currently in use (opened or locked by a client) cannot be deleted.
returns: 0 if the file is deleted successfully.
		-1 if the file does not exist
		-2 if the file is in use or locked
		-3 if the file cannot be deleted for any other reason
*/
int FileSystem::deleteFile(char* filename, int fnameLen)
{
	if (!isValidName(filename, fnameLen)) return -3;
	else if (!pathExists(filename, fnameLen)) return -1;
	else if (locked.find(filename) != locked.end()) return -2;
	else
	{
		char type = '%';
		int inodeBlockNum = getBlockNumber(filename, fnameLen, type);
		if (type != 'f') return -3; //GB: target is not a file

		char fileInode[64];

		myPM->readDiskBlock(inodeBlockNum, fileInode);//get the finode then get check for extended files

		/* -------------------------------- Deallocating Direct Blocks -------------------------------- */

		int pos = 6;
		for (int i = 0; i < 3; i++)
		{
			int dataBlockNum = myDM->charToInt(pos, fileInode);//get the index's of the allocated mem blocks
			if (dataBlockNum != 0) myPM->returnDiskBlock(dataBlockNum);
			pos += 4;
		}

		/* ------------------------------- Deallocating Indirect Blocks ------------------------------- */

		int indirectInodeBlockNum = myDM->charToInt(fInodeIndirectBlock, fileInode);
		if (indirectInodeBlockNum > 0) //GB: indirect blocks allocated
		{
			char indirectInode[64];
			myPM->readDiskBlock(indirectInodeBlockNum, indirectInode);
			pos = 0;
			for (int i = 0; i < 16; i++)
			{
				int dataBlockNum = myDM->charToInt(pos, indirectInode);
				if (dataBlockNum > 0) myPM->returnDiskBlock(dataBlockNum);
				pos += 4;
			}
		}

		/* ---------------------------------- Deallocating the Inode ---------------------------------- */

		myPM->returnDiskBlock(inodeBlockNum);

		/* ----------------------------- Removing Address From Directory ------------------------------ */
		
		char dirInode[64];
		int dirInodeBlkNum = 1;
		if (fnameLen == 2) //GB: in root directory
		{
			myPM->readDiskBlock(dirInodeBlkNum, dirInode);
		}
		else //GB: in a subdirectory
		{
			//GB: path to file directory
			char dirPath[fnameLen - 2];
			for (int i = 0; i < (fnameLen - 2); i++) dirPath[i] = filename[i];
			dirInodeBlkNum = getInode(dirPath, (fnameLen - 2), 'd', dirInode);
		}

		//GB: used to deallocate disk space if, upon removal of the file inode pointer 
		//    from an extension block of the parent directory, the extension block is
		//    found empty.
		int extendedDirBlock = -1;
		int extendedDirBlockParent = -1;
		char target = filename[fnameLen - 1];
		bool done = false;
		while (!done)
		{
			for (int pos = 0; pos < 60; pos += 6)
			{
				if (dirInode[pos + 1] != '#')
				{
					if (dirInode[pos] == target)
					{
						for (int i = 0; i < 6; i++) dirInode[pos + i] = '#';
						done = true;
						break;
					}
				}
			}
			if (!done)
			{
				extendedDirBlockParent = dirInodeBlkNum;
				dirInodeBlkNum = myDM->charToInt(60, dirInode);
				if (dirInodeBlkNum != 0) //GB: the directory continues in another block
				{
					extendedDirBlock = dirInodeBlkNum;
					myPM->readDiskBlock(dirInodeBlkNum, dirInode);
				}
				else return -1; //GB: not found
			}
		}

		//GB: if deleted from an extend directory block, check if the block is now empty
		//    if it is, deallocate the space
		if (extendedDirBlock != -1)
		{
			bool empty = true;
			for (int i = 0; i < 64; i++)
			{
				if (dirInode[i] != '#')
				{
					empty = false;
					break;
				}
			}
			if (empty)
			{
				myPM->returnDiskBlock(dirInodeBlkNum);
				dirInodeBlkNum = extendedDirBlockParent;
				myPM->readDiskBlock(dirInodeBlkNum, dirInode);
				for (int i = 60; i < 64; i++) dirInode[i] = '#';
			}
		}

		myPM->writeDiskBlock(dirInodeBlkNum, dirInode);

		return 0;
	}
}

/*
Deletes the directory whose name is pointed to by dirname.
Only an empty directory can be deleted.
returns: 0 if the directory is deleted successfully.
		-1 if the directory does not exist
		-2 if the directory is not empty
		-3 if the directory cannot be deleted for any other reason
*/
int FileSystem::deleteDirectory(char* dirname, int dnameLen)
{
	if(!isValidName(dirname, dnameLen)) return -3;
	else if (!pathExists(dirname, dnameLen)) return -1;
	else
	{
		char type = '%';
		int dirInodeBlkNum = getBlockNumber(dirname, dnameLen, type);
		if (type != 'd') return -1; //GB: target is not a directory

		char dirInode[64];
                 for(int c = 0; c < 64 ; c++) dirInode[c] = '#';
		myPM->readDiskBlock(dirInodeBlkNum, dirInode); //get the directory inode then get check for entended files

		/* ------------------------------- Check if Directory is Empty -------------------------------- */

		bool empty = true, done = false;
		//GB: used to delete extended directory addresses, may no longer be needed since
		//    empty directory extension blocks now delete themselves
		vector<int> directoryAddresses;
		int addressCount = 0;
		while (!done)
		{
         
                    
			for (int i = 0; i < 60; i += 6)
			{
				if (dirInode[i] != '#')
				{
					empty = false;
					done = true;
					break;
				}
			}
			if (!done)
			{
				int nextBlkNum = myDM->charToInt(60, dirInode);
				if (nextBlkNum > 0)
				{
					myPM->readDiskBlock(nextBlkNum, dirInode);
					directoryAddresses.push_back(nextBlkNum);
					++addressCount;
				}
				else
				{
					done = true;
				}
			}

		}
		if (!empty) return -2;

		/* --------------------------- Delete Extended Directory Addresses ---------------------------- */

		for (int i = 0; i < addressCount; i++)
		{
			int address = directoryAddresses.back();
			directoryAddresses.pop_back();
			myPM->returnDiskBlock(address);
		}

		/* ------------------------------ Delete Main Directory Address ------------------------------- */
		
		myPM->returnDiskBlock(dirInodeBlkNum);
		
		/* -------------------------- Removing Address From Parent Directory -------------------------- */

		char parentDirInode[64];
		int parentInodeBlkNum = 1;
		if (dnameLen == 2) //GB: in root directory
		{
			myPM->readDiskBlock(parentInodeBlkNum, parentDirInode);
		}
		else //GB: in a subdirectory
		{
			//GB: path to file directory
			char dirPath[dnameLen - 2];
			for (int i = 0; i < (dnameLen - 2); i++) dirPath[i] = dirname[i];
			parentInodeBlkNum = getInode(dirPath, (dnameLen - 2), 'd', parentDirInode);
		}

		//GB: used to deallocate disk space if, upon removal of the directory pointer 
		//    from an extension block of the parent directory, the extension block is
		//    found empty.
		int extendedDirBlock = -1;
		int extendedDirBlockParent = -1;

		char target = dirname[dnameLen - 1];
		done = false;
		while (!done)
		{
			for (int pos = 0; pos < 60; pos += 6)
			{
				if (parentDirInode[pos + 1] != '#')
				{
					if (parentDirInode[pos] == target)
					{
						for (int i = 0; i < 6; i++) parentDirInode[pos + i] = '#';
						done = true;
						break;
					}
				}
			}
			if (!done)
			{
				extendedDirBlockParent = parentInodeBlkNum;
				parentInodeBlkNum = myDM->charToInt(60, parentDirInode);
				if (parentInodeBlkNum != 0) //GB: the directory continues in another block
				{
					extendedDirBlock = parentInodeBlkNum;
					myPM->readDiskBlock(parentInodeBlkNum, parentDirInode);
				}
				else return -1; //GB: not found
			}
		}
		//GB: if deleted from an extend directory block, check if the block is now empty
		//    if it is, deallocate the space
		if (extendedDirBlock != -1)
		{
			bool empty = true;
			for (int i = 0; i < 64; i++)
			{
				if (parentDirInode[i] != '#')
				{
					empty = false;
					break;
				}
			}
			if (empty)
			{
				myPM->returnDiskBlock(parentInodeBlkNum);
				parentInodeBlkNum = extendedDirBlockParent;
				myPM->readDiskBlock(parentInodeBlkNum, parentDirInode);
				
				for (int i = 60; i < 64; i++) parentDirInode[i] = '#';
			}
		}

		myPM->writeDiskBlock(parentInodeBlkNum, parentDirInode);
		return 0;
	}
}

/*
Opens a file whose name is pointed to by filename.
If mode == 'r' -> the file is opened for read only
		   'w' -> the file is opened for write only
		   'm' -> the file is opened for read and write.
An existing file cannot be opened if:
	>The file is locked and lock_id doesn't match with lock_id returned by the lockFile function
	when the file was locked
	>The file is not locked and lock id != -1.
If the file is opened successfully, an rw pointer (read-write pointer) is associated with this file descriptor.
This rw pointer is used by some of the operations described later for determining the access point in a file.
The initial value of an rw pointer is 0 (beginning of the file).
returns: A unique positive integer (file descriptor) if the file is opened successfully.
		-1 if the file does not exist
		-2 if mode is invalid
		-3 if the file cannot be opened because of locking restrictions
		-4 for any other reason
*/
int FileSystem::openFile(char* filename, int fnameLen, char mode, int lockId)
{
	if (!isValidName(filename, fnameLen)) return -1;
	if ((mode != 'r') && (mode != 'w') && (mode != 'm')) return -2; //checks for a valid mode

	if (pathExists(filename, fnameLen))
	{
		char type = '%';
		getBlockNumber(filename, fnameLen, type);
		if (type != 'f') return -1; //GB: target is not a file

		//GB: if (the file is locked & the lockId is valid) or the lockId is -1
		bool isLocked = (locked.find(filename) != locked.end());
		if ((isLocked && locked[filename] == lockId) || (lockId == -1 && !isLocked))
		{
			if (lockId == -1) locked[filename] = lockId;

			openFileData openedFile;
			openedFile.filename = filename[fnameLen - 1];
			openedFile.filePath = filename;
			openedFile.filePathLen = fnameLen;
			openedFile.lockID = lockId;
			openedFile.mode = mode;
			openedFile.rwpointer = 0;

			openedFiles[++fileDesc] = openedFile;

			return fileDesc;
		}
		else return -3;
	}
	else return -1; //File does not exist
}

/*
Closes the file with file descriptor filedesc.
returns: 0 if successful.
		-1 if the file descriptor is invalid
		-2 for any other reason
*/
int FileSystem::closeFile(int fileDesc)
{
	if (openedFiles.find(fileDesc) == openedFiles.end())
	{
		return -1; //GB: no file matching fileDesc
	}
	else
	{

		if (locked[openedFiles[fileDesc].filePath] == -1)
		{
			locked.erase(openedFiles[fileDesc].filePath);
		}

		openedFiles.erase(fileDesc);

		return 0;
	}
}

/*
Reads a file whose file descriptor is filedesc.
len is the number of bytes to be read from the buffer pointed to by data.
May read less number of bytes than length if end of file is reached earlier.
Operates from the byte pointed to by the rw pointer.
return: number of bytes read if successful.
	   -1 if the file descriptor is invalid
	   -2 if length is negative
	   -3 if the operation is not permitted
	   -4 read failed
After the read is done, the rw pointer is updated to point to the byte following the last byte read.
*/
int FileSystem::readFile(int fileDesc, char* data, int len)
{
	if (openedFiles.find(fileDesc) == openedFiles.end()) return -1;
	else if (len <= 0) return -2;
	else if (openedFiles[fileDesc].mode == 'w') return -3; //GB: mode is write

	int fileSize = getFileLength(openedFiles[fileDesc].filePath, openedFiles[fileDesc].filePathLen);
	if (fileSize == -1) return -4;

	char inode[64], indirectInode[64];
	int inodeBlkNum = getInode(openedFiles[fileDesc].filePath, openedFiles[fileDesc].filePathLen, 'f', inode);

	int rwStart = openedFiles[fileDesc].rwpointer;

	int bytesLeftInFile = fileSize - rwStart;

	//GB: number of blocks that need to be read
	int blocks = ((len + rwStart) / 64) + ((((len + rwStart) % 64) > 0) ? 1 : 0);

	int startingBlock = rwStart / 64;

	int indirectInodeBlockNum = -1;
	if ((startingBlock + blocks) > 3)
	{
		indirectInodeBlockNum = myDM->charToInt(fInodeIndirectBlock, inode);
		myPM->readDiskBlock(indirectInodeBlockNum, indirectInode);
	}

	int dataBlockLen = len;
	if ((rwStart + len) > 64)
	{
		dataBlockLen = 64 - (rwStart % 64);
		len -= dataBlockLen;
	}

	int read = 0;
	for (int block = startingBlock; block < blocks; block++)
	{
		int blknum = -1, pos;
		if (block < 3) //GB: direct block pointers
		{
			pos = fileDirectBlocks + (4 * block);
			blknum = myDM->charToInt(pos, inode);
		}
		else if (block < 19) //GB: indirect block pointers
		{
			pos = 0 + (4 * (block - 3));
			blknum = myDM->charToInt(pos, indirectInode);
		}
		else break; //GB: out of bounds

		char buffer[64];
		myPM->readDiskBlock(blknum, buffer);

		for (int i = 0; i < dataBlockLen; i++)
		{
			if (bytesLeftInFile == 0) return read;
			data[read++] = buffer[(openedFiles[fileDesc].rwpointer++) - (64 * block)];
			--bytesLeftInFile;
		}

		dataBlockLen = (len > 64) ? 64 : len;
		len -= dataBlockLen;
	}

	return read;
}

/*
Perform write operations on a file whose file descriptor is filedesc.
len is the number of bytes to be written into the buffer pointed to by data.
Overwrites the existing data in the file and may increase the size of the file.
Operates from the byte pointed to by the rw pointer.
return: number of bytes written if successful.
	   -1 if the file descriptor is invalid
	   -2 if length is negative
	   -3 if the operation is not permitted
After the write is done, the rw pointer is updated to point to the byte following the last byte written.
*/
int FileSystem::writeFile(int fileDesc, char* data, int len)
{
	/* ---------------------------------- Error Prevention ---------------------------------- */

	if (openedFiles.find(fileDesc) == openedFiles.end()) return -1;
	else if (len <= 0) return -2;
	else if (openedFiles[fileDesc].mode == 'r') return -3; //GB: mode is read

	/* ---------------------------------- Block Allocation ---------------------------------- */

	//GB: used to determine how size is adjusted in the inode at the conclusion of this 
	//    function's execution
	int rwStart = openedFiles[fileDesc].rwpointer;

	//GB: number of blocks required to fit 'data' in the file starting from the rwpointer
	int blocks = ((len + rwStart) / 64) + ((((len + rwStart) % 64) > 0) ? 1 : 0);

	char inode[64], indirectInode[64];
	int fileInodeBlockNum = getInode(openedFiles[fileDesc].filePath, openedFiles[fileDesc].filePathLen, 'f', inode);

	int fileSizeInitial = myDM->charToInt(2, inode);
	if ((rwStart + len) > fileSizeInitial) //GB: the write is going beyond the current length of the file
	{
		int rv = additionalBlockAllocation(fileInodeBlockNum, blocks);
		if (rv == -1) return -3;
	}
	myPM->readDiskBlock(fileInodeBlockNum, inode);

	int indirectInodeBlockNum = -1;
	if (blocks > 3) //GB: not enough direct block pointers to satisfy the file space requirements
	{
		indirectInodeBlockNum = myDM->charToInt(fInodeIndirectBlock, inode);
		myPM->readDiskBlock(indirectInodeBlockNum, indirectInode);
	}

	/* -------------------------------------- Writing --------------------------------------- */

	int startingBlock = rwStart / 64;
	//GB: determines the amount of data written to individual blocks
	int dataBlockLen = len;
	if ((rwStart + len) > 64)
	{
		dataBlockLen = 64 - (rwStart % 64);
		len -= dataBlockLen;
	}
	int written = 0;
	for (int block = startingBlock; block < blocks; block++)
	{
		//GB: get blknum, will need to be adjusted to actually handle multiple blocks
		int blknum = -1, pos;
		if (block < 3) //GB: direct block pointers from inode
		{
			pos = fileDirectBlocks + (4 * block);
			blknum = myDM->charToInt(pos, inode);
		}
		else if (block < 19) //GB: indirect inode block pointers
		{
			pos = 4 * (block - 3);
			blknum = myDM->charToInt(pos, indirectInode);
		}
		else break; //GB: too many blocks

		char buffer[64];
		myPM->readDiskBlock(blknum, buffer);

		for (int i = 0; i < dataBlockLen; i++)
		{
			buffer[(openedFiles[fileDesc].rwpointer++) - (64 * block)] = data[written++];
		}

		//cout << blknum << endl;
		int r = myPM->writeDiskBlock(blknum, buffer);
               // cout << r << endl;
		if (r != 0) return -3;

		dataBlockLen = (len > 64) ? 64 : len;
		len -= dataBlockLen;
	}

	/* ----------------------------- Updating Filesize in Inode ----------------------------- */
	int fileSize = myDM->charToInt(2, inode);
	if ((rwStart + written) > fileSize)
	{
		myDM->intToChar(2, (rwStart + written), inode);
		myPM->writeDiskBlock(fileInodeBlockNum, inode);
	}
	return written;
}

/*
Appends on a file whose file descriptor is filedesc.
len is the number of bytes to be appended into the buffer pointed to by data.
Appends the data at the end of the file.
return: number of bytes appended if successful.
	   -1 if the file descriptor is invalid
	   -2 if length is negative
	   -3 if the operation is not permitted
After the append is done, the rw pointer is updated to point to the byte following the last byte appended.
*/
int FileSystem::appendFile(int fileDesc, char* data, int len)
{
	if (openedFiles.find(fileDesc) == openedFiles.end()) return -1;
	else if (openedFiles[fileDesc].mode == 'r') return -3;
        
        char buff[64];
        int blk = getInode(openedFiles[fileDesc].filePath, openedFiles[fileDesc].filePathLen, 'f', buff);
        myPM->readDiskBlock(blk, buff);
        int size = myDM->charToInt(2, buff);
        if(len+size > 1216)
            return -3;//run out of space
        

	int length = getFileLength(openedFiles[fileDesc].filePath, openedFiles[fileDesc].filePathLen);

	openedFiles[fileDesc].rwpointer = length;

	return writeFile(fileDesc, data, len);
}

/*
Modifies the rw pointer of the file whose file descriptor is filedesc.
The rw pointer is moved offset bytes forward if flag = 0. Otherwise, it is set to byte number offset in the file.
returns: 0 if successful.
		-1 if the file descriptor, offset, or flag is invalid
		-2 if an attempt to go outside the file bounds is made (end of file or beginning of file)
A negative offset is valid only when flag is zero.
*/
int FileSystem::seekFile(int fileDesc, int offset, int flag)
{
	if (openedFiles.find(fileDesc) == openedFiles.end()) return -1;

	// Read in file Inode
	char fInode[myPM->getBlockSize()];
	char* path = openedFiles[fileDesc].filePath;
	int len = openedFiles[fileDesc].filePathLen;
	int check = myPM->readDiskBlock(getBlockNumber(path, len), fInode);
	if (check != 0)
	{
		return -4;
	}

	int fileSize = myDM->charToInt(2, fInode); // file size at byte 2

	if (flag == 0)
	{
		if (((openedFiles[fileDesc].rwpointer + offset) > fileSize))
		{
			return -2; // out of bound
		}
		if ((openedFiles[fileDesc].rwpointer + offset) < 0)
		{
			return -2;

		}
		openedFiles[fileDesc].rwpointer += offset;
	}
	else
	{
		if (offset < 0) return -1;
		else if (offset > fileSize) return -2; // out of bounds

		openedFiles[fileDesc].rwpointer = offset;

		if (offset == fileSize)
		{
			openedFiles[fileDesc].rwpointer = offset;
			return -2; // get to the end of the file. return -2 for writing file
		}

	}
	return 0;
}

/*
Changes the name of the file whose name is pointed to by filename1 to the name pointed to by filename2.
returns: 0 if successful.
		-1 invalid filename
		-2 if the file does not exist
		-3 if there already exists a file whose name is the same as the name pointed to by fname2
		-4 if file is opened or locked
		-5 for any other reason
A note, you can rename a directory with this operation.
*/
int FileSystem::renameFile(char* filename1, int fnameLen1, char* filename2, int fnameLen2)
{
	if (!isValidName(filename2, fnameLen2)) return -1;
	else if (!pathExists(filename1, fnameLen1)) return -2;
	else if (pathExists(filename2, fnameLen2)) return -3;
	//GB: hopefully this will work, lockNum should be set to -1 when a file is opened without being locked
	else if (locked.find(filename1) != locked.end()) return -4;

	/* ------------------------------------- Get Target's Directory ------------------------------------- */

	char buffer[64];
	int dirInodeBlkNum = 1;
	if (fnameLen1 == 2) //GB: in root directory
	{
		myPM->readDiskBlock(dirInodeBlkNum, buffer);
	}
	else //GB: in a subdirectory
	{
		//GB: path to file directory
		char dirPath[fnameLen1 - 2];
		for (int i = 0; i < (fnameLen1 - 2); i++) dirPath[i] = filename1[i];
		dirInodeBlkNum = getInode(dirPath, (fnameLen1 - 2), 'd', buffer);
	}

	char typeCheck = '%';
	int fileInodeBlkNum = getBlockNumber(filename1, fnameLen1, typeCheck);

	/* -------------------------------- Change Target's Name in Directory ------------------------------- */
	
	char target = filename1[fnameLen1 - 1];
	bool done = false;
	while (!done)
	{
		for (int pos = 0; pos < 60; pos += 6)
		{
			if (buffer[pos + 1] != '#')
			{
				if (buffer[pos] == target)
				{
					buffer[pos] = filename2[fnameLen2 - 1];
					done = true;
					break;
				}
			}
		}
		if (!done)
		{
			dirInodeBlkNum = myDM->charToInt(60, buffer);
			if (dirInodeBlkNum != 0) //GB: the directory continues in another block
			{
				myPM->readDiskBlock(dirInodeBlkNum, buffer);
			}
			else return -1; //GB: not found
		}
	}
	myPM->writeDiskBlock(dirInodeBlkNum, buffer);

	/* --------------------------------- If File, Change Name In Inode ---------------------------------- */

	
	if (typeCheck == 'f')
	{
		myPM->readDiskBlock(fileInodeBlkNum, buffer);
		buffer[0] = filename2[fnameLen2 - 1];
		int check = myPM->writeDiskBlock(fileInodeBlkNum, buffer);
	}

	return 0;
}

/*
attributeID: 0 for Creator Name | 1 for Color
attribute: the container the attribute will be read into
returns: 0 if successful
        -1 filename doesn't exist
		-2 if fnameLen is less than 0
		-3 if attributeID is invalid
		-4 if attribute at attributeID has no value
*/
int FileSystem::getAttribute(char* filename, int fnameLen, int attributeID, char* attribute)
{
	if (!pathExists(filename, fnameLen)) return -1;
	else if (fnameLen < 0) return -2;
	else if (attributeID != 0 && attributeID != 1) return -3;
	char inode[64]; 
	int blkNum = getInode(filename, fnameLen, 'f', inode);
	int attributes[2] = { attribute0, attribute1 };
	if (inode[attributes[attributeID]] == '#') return -4;

	int upperLimit = (attributeID == 0) ? attribute1 : 64;

	int index = 0;
	for (int pos = attributes[attributeID]; pos < upperLimit; pos++)
	{
		if (inode[pos] == '#') break;
		attribute[index++] = inode[pos];
	}
	
	return 0;
}

/*
attributeID: 0 for Creator Name | 1 for Color
attribute: the container the attribute will be read from
attributeLen: the length of the attribute to be added
returns: the number of bytes written from attribute if successful
		-1 filename doesn't exist
		-2 if fnameLen is less than 0
		-3 if attributeID is invalid
		-4 is attributeLen is invalid
		-5 if the attribute is too large to add to the inode
*/
int FileSystem::setAttribute(char* filename, int fnameLen, int attributeID, char* attribute, int attributeLen)
{
	//GB: might need to check if the file is locked or open, not sure

	if (!pathExists(filename, fnameLen)) return -1;
	else if (fnameLen < 0) return -2;
	else if (attributeID != 0 && attributeID != 1) return -3;
	else if (attributeLen < 1 || attributeLen > 18) return -4;
	char inode[64];
	int blkNum = getInode(filename, fnameLen, 'f', inode);


	int written = 0;
	if (attributeID == 0)
	{
		if (inode[attribute0] != '#') //GB: attributePresent
		{
			for (int pos = attribute0; pos < attribute1; pos++)
			{
				if (inode[pos] != '#') inode[pos] = '#';
				else break;
			}
		}
		//GB: start writing
		int pos = attribute0;
		while (written < attributeLen && pos < attribute1)
		{
			inode[pos++] = attribute[written++];
		}
	}
	if (attributeID == 1)
	{
		if (inode[attribute1] != '#') //GB: attributePresent
		{
			for (int pos = attribute1; pos < 64; pos++)
			{
				if (inode[pos] != '#') inode[pos] = '#';
				else break;
			}
		}
		//GB: start writing
		int pos = attribute1;
		while (written < attributeLen && pos < 64)
		{
			inode[pos++] = attribute[written++];
		}
	}
	myPM->writeDiskBlock(blkNum, inode);

	return written;
}

/*
returns the number of additional blocks successfully allocated
*/
int FileSystem::additionalBlockAllocation(int inodeBlkNum, int totalBlocks)
{
	char inode[64], indirectInode[64];
	myPM->readDiskBlock(inodeBlkNum, inode);

	if (totalBlocks > 19) totalBlocks = 19;

	/* ------------------------------- Count Allocated Blocks ------------------------------- */

	int directBlockAddress = -1;
	int allocatedBlocks = 0;
	int pos = fileDirectBlocks;
	while (directBlockAddress != 0 && allocatedBlocks < 3) //GB: counting direct blocks
	{
		directBlockAddress = myDM->charToInt(pos, inode);
		if (directBlockAddress != 0)
		{
			allocatedBlocks++;
			pos += 4;
		}
	}
	//GB: the address of the indirect inode block
	int indirectBlockNum = -1;
	//GB: a temp variable for the indirect inode block address as well as the addresses contained within
	int value = -1;
	if (totalBlocks > 3) //GB: indirect block addresses needed
	{
		value = myDM->charToInt(fInodeIndirectBlock, inode);
		if (value != 0) //GB: indirect inode exists
		{
			indirectBlockNum = value; //GB: assign the value of indirectBlockNum
			pos = 0;
			myPM->readDiskBlock(indirectBlockNum, indirectInode);
			while (value != 0 && allocatedBlocks < 19)
			{
				value = myDM->charToInt(pos, indirectInode);
				if (value != 0) //GB: indirect block addresses continue
				{
					allocatedBlocks++;
					pos += 4;
				}
			}
		}
		else //GB: create indirect block
		{
			indirectBlockNum = myPM->getFreeDiskBlock();
			if (indirectBlockNum < 0) return -1; //GB: no space available for the indirect inode
			myDM->intToChar(fInodeIndirectBlock, indirectBlockNum, inode);
			myPM->readDiskBlock(indirectBlockNum, indirectInode); 
		}
	}

	/* ----------------------------------- Allocate Blocks ---------------------------------- */

	int allocated = 0;
	int initialNumBlocks = allocatedBlocks;
	while ((allocatedBlocks < totalBlocks) && (allocated < (19 - initialNumBlocks)))
	{
		int block = myPM->getFreeDiskBlock();

		if (block < 1) break;
		if (allocatedBlocks < 3)
		{
			myDM->intToChar((fileDirectBlocks + (4 * allocatedBlocks)), block, inode);
			++allocatedBlocks;
			++allocated;
		}
		else if (allocatedBlocks < 19)
		{
			myDM->intToChar((4 * (allocatedBlocks - 3)), block, indirectInode);
			++allocatedBlocks;
			++allocated;
		}
	}

	myPM->writeDiskBlock(inodeBlkNum, inode);
	if (indirectBlockNum != -1) myPM->writeDiskBlock(indirectBlockNum, indirectInode);

	return allocated;
}

/***
 * Takes The Directory Block Num that needs to be extended
 * and a block that will be allocated
 * Return Vals:
 *      0: if successful
 *      -4:if fail
 ***/
int FileSystem::extendDirectory(int inodeBlkNum, int newBlk)
{
	char dirInode[myPM->getBlockSize()], newDir[myPM->getBlockSize()];

	///////////////////Read in the Two Buffers//////////////////
	int check = myPM->readDiskBlock(inodeBlkNum, dirInode);
	if (check < 0)
	{
		return -4;//read error
	}
	check = myPM->readDiskBlock(newBlk, newDir);
	if (check < 0)
	{
		return -4;//read error
	}
	///////////////////Put Block Pointer In Directory//////////////


	myDM->intToChar(60, newBlk, dirInode);


	/////////////////Format The New Block/////////////////////////

	//Cleanout any bad Data
	for (int i = 0; i < myPM->getBlockSize(); i++)
	{
		newDir[i] = '#';
	}

	////////////////Write The buffers////////////////////////////
	check = myPM->writeDiskBlock(inodeBlkNum, dirInode);
	if (check < 0)
	{
		return -4;//write error
	}
	
	check = myPM->writeDiskBlock(newBlk, newDir);
	if (check < 0) return -4;//write error
	return 0;
}



/*
SB: Added 11-27-19
Checking if the file name is valid
returns true if it is valid false otherwise
*/
bool FileSystem::isValidName(char* name, int length)
{
	if ((length % 2) != 0)
	{
		/*
		SB: Checks if the length is a multiple of 2 becuase the name
		must be a combination of a '/' and an alphabet character
		returns false if not multiple of 2
		*/
		return false;
	}

	for (int i = 0; i < length; i++)
	{
		//SB: Checks that every other character is a '/' character
		//SB: returns false if not a valid name
		if (i % 2 == 0)
		{
			if (name[i] != '/')
			{
				return false;
			}
		}
		else //SB: checks if it is a valid alphabet character returns false if it is not valid
		{
			//SB:checks if the char given is between the ASCII vals A and z
			if ((name[i] < 65) || (name[i] > 122))
			{
				return false;
			}
			//SB:There are 6 non-alphabet characters so we need to make sure those arent in
			if ((name[i] > 90) && (name[i] < 97)) {
				return false;
			}
		}
	}
	//SB: if it passes all checks return true
	return true;
}

//GB: I really don't like this, but until I figure something else out it stays
bool FileSystem::isOpen(char* path, int pathLen)
{
	char name = path[pathLen - 1];
	map<int, openFileData>::iterator open;
	bool match = false;
	for (open = openedFiles.begin(); open != openedFiles.end(); open++)
	{
		if (open->second.filename == name)
		{
			match = true;
			for (int i = 0; i < pathLen; i++)
			{
				if (path[i] != open->second.filePath[i])
				{
					match = false;
					break;
				}
			}

			if (match == true) return match;
		}
	}
	return false;
}

bool FileSystem::pathExists(char* path, int len)
{
	int temp = getBlockNumber(path, len);
	return (temp > 0) ? true : false;;
}

/*
returns: the block number of a file/directory inode if successful
		-1: if file path not found
		-2: invalid file length
type is an optional parameter used by `getIndode()`
*/
int FileSystem::getBlockNumber(char* path, int len, char& type)
{
	//GB: _type_decoy allows for type to be an optional parameter while also 
	//    allowing it to be passed-by-reference

	if (len <= 0) return -2;

	char dirInode[64];
	myPM->readDiskBlock(1, dirInode);
	for (int j = 0; j < len; j += 2)
	{
		char target = path[j + 1];
		bool done = false;
		int block = -1;
		while (!done)
		{
			for (int pos = 0; pos < 60; pos += 6)
			{
				if (dirInode[pos + 1] == '#') //'r' will need to be changed to '#'
				{
					//GB: I chose dirInode[pos + 1] because the type flag will never be an 'r'
					//    but a file could easily be named 'r'

					//return -1; //GB: end of directory inode, target not found
				}
				else if (dirInode[pos] == target)
				{
					if (j == len - 2) //GB: target found
					{
						type = dirInode[pos + 1];

						block = myDM->charToInt(pos + 2, dirInode);
						return block;
					}
					else
					{
						if (dirInode[pos + 1] == 'd') //GB: if target is a directory
						{
							block = myDM->charToInt(pos + 2, dirInode);
							myPM->readDiskBlock(block, dirInode);
							done = true;
							break;
						}
						else return -1; //GB: target is a file, making the filepath invalid
					}
				}
			}
			if (block == -1) //GB: not found in the first block of the directory
			{

				int nextDirBlock = myDM->charToInt(60, dirInode);
				if (nextDirBlock != 0) //GB: the directory continues in another block
				{

					myPM->readDiskBlock(nextDirBlock, dirInode);
				}
				else return -1; //GB: not found
			}
		}
	}
}

/*
 * SB: We need a way to navigate thru directories
 * And to find a block num for a given path
 * this can double-y be a check to see if the given path even exists
 * The main purpose of this is to update the previous direcotry with data

 params: path(the file/directory path)
		 len(the character length of the path)
		 type(the type of the target destination)
			 ('d' : directory) or ('f' : file)
		 inodeBuffer(the buffer to be filled by the inode)

 returns: block number of the inode if successful
		 -1: if not found
		 -2: negative len
		 -3: invalid type
 */
int FileSystem::getInode(char* path, int len, char type, char* inodeBuffer)
{
	if (len <= 0) return -3;
	else if (type != 'f' && type != 'd') return -4;

	char typeCheck = '%';

	int block = getBlockNumber(path, len, typeCheck);
	if (block == -1) return -1; //GB: filepath doesn't exist
	if (type != typeCheck) return -2; //GB: filepath exists for a different type

	int rv = myPM->readDiskBlock(block, inodeBuffer);
	return block;
}

/*
returns file length if successful
	   -1 otherwise
*/
int FileSystem::getFileLength(char* path, int len)
{
	char buffer[64];
	int inodeBlockNum = getInode(path, len, 'f', buffer);
	if (inodeBlockNum < 0) return inodeBlockNum;
	return myDM->charToInt(2, buffer);
}

/*
 * SB: This function is to look through a given directory for a given name
 *
 * Returns:
 *      the blocknum if successful
 *      -1 if DNE
 *      -2 for wrong type
 *      -4 of any other reason
 */
int FileSystem::searchDirectory(char* name, int& blknum, char type)
{
	int i = 0, block = blknum;
	bool done = false;
	char buffer[myPM->getBlockSize()];
	//cout << name << endl;
	//cout << blknum << endl;
	//for(int i =0;i<64;i++) cout << buffer[i];
	//cout<<endl;
	int check = myPM->readDiskBlock(block, buffer);
	if (check < 0) return -4; //Anything less than zero is a read failure

	while (!done)
	{
		if (buffer[i] == name[1])//Checks if the current one being read is it
		{
			if (buffer[i + 1] != type) return -2; //Wrong Type
			else //Types Match
			{
				done = true;
				blknum = block;
				return i;
			}
		}

		/*
		 * Right here add a thing for if there is a connecting direcotry block
		 * that will be when i == 60
		 */
		if (i >= 60) return -4;//For right now it escapes if there is a connection

		i += 6;

	}

}

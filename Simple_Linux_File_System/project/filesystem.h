#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <map>
#include <vector>

#define fInodeName 0
#define fInodeType 1
#define fInodeSize 2
#define fileDirectBlocks 6
#define fInodeIndirectBlock 18
#define attribute0 22
#define attribute1 43

class openFileData
{
public:
	char filename;
	char* filePath;
	int filePathLen;
	char mode;
	int lockID;
	int rwpointer;
};

static char _type_decoy;
class FileSystem {
  DiskManager *myDM;
  PartitionManager *myPM;
  char fileSystemName;
  int fileSystemSize;
  
  map<int, openFileData> openedFiles;
  map<char*, int> locked;
  int fileDesc; //index of the opend fil
  int lockIDCounter;
  
  /* declare other private members here */
  int getInode(char* path, int len, char type, char* inodeBuffer);
  int getFileLength(char* path, int len);
  int searchDirectory(char* path, int &blockNum, char type);
  int additionalBlockAllocation(int inodeBlkNum, int additionalBlocks);
  char* createBuffer(int size = 64);
  bool pathExists(char* path, int len);
  bool isOpen(char* path, int len);
  int getBlockNumber(char* path, int pathLength, char& type = _type_decoy);
  int extendDirectory(int inodeBlkNum, int newBlk);

  public:
	  
    FileSystem(DiskManager *dm, char fileSystemName);
    ~FileSystem(){delete myPM;}
    bool isValidName(char* name, int length);
    int createFile(char *filename, int fnameLen);
    int createDirectory(char *dirname, int dnameLen);
    int lockFile(char *filename, int fnameLen);
    int unlockFile(char *filename, int fnameLen, int lockId);
    int deleteFile(char *filename, int fnameLen);
    int deleteDirectory(char *dirname, int dnameLen);
    int openFile(char *filename, int fnameLen, char mode, int lockId);
    int closeFile(int fileDesc);
    int readFile(int fileDesc, char *data, int len);
    int writeFile(int fileDesc, char *data, int len);
    int appendFile(int fileDesc, char *data, int len);
    int seekFile(int fileDesc, int offset, int flag);
    int renameFile(char *filename1, int fnameLen1, char *filename2, int fnameLen2);
	int getAttribute(char* filename, int fnameLen, int attributeID, char* attribute);
    int setAttribute(char* filename, int fnameLen, int attributeID, char* attribute, int attributeLen);

    /* declare other public members here */

};

#endif
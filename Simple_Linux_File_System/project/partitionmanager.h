/* 
 * partionmanager.h
 * Header file for the partion manager
 */

/*
 * Modiefied By S Blair
 * Date: Nov 11, 2019
 * What: Added A bit vector variable and fixed compiler errors
 */

#include "bitvector.h"
#include "diskmanager.h"
#ifndef PARTITIONMANAGER_H
#define PARTITIONMANAGER_H
class PartitionManager {
  DiskManager *myDM;


  public:
    char myPartitionName;
    int myPartitionSize;
    PartitionManager(DiskManager *dm, char partitionname, int partitionsize);
    PartitionManager();
    ~PartitionManager();
    int readDiskBlock(int blknum, char *blkdata);
    int writeDiskBlock(int blknum, char *blkdata);
    int getBlockSize();
    int getFreeDiskBlock();
    int returnDiskBlock(int blknum);
    void updateBitVectorBlock();
    BitVector* myBitVector;
};

#endif